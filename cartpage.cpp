#include "cartpage.h"
#include "ui_cartpage.h"
#include "cartmanager.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateTime>

CartPage::CartPage(CartManager *manager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartpage)
    , m_manager(manager)
{
    ui->setupUi(this);

    // === 新布局：限制 listWidget 高度，保持按钮不被挤掉 ===
    QVBoxLayout *rootLayout = new QVBoxLayout;
    ui->centralwidget->setLayout(rootLayout);
    rootLayout->setContentsMargins(10, 10, 10, 10);
    rootLayout->setSpacing(8);


    // 顶部按钮布局（返回 + 新按钮）
    QHBoxLayout *topLayout = new QHBoxLayout;

    QPushButton *btnBack = new QPushButton("返回", this);
    btnBack->setStyleSheet("background:#FFC107; padding:10px; font-size:20px;border-radius: 10px;");
    btnBack->setFixedHeight(50);

    // 点击返回：关闭小票 + 返回主界面
    connect(btnBack, &QPushButton::clicked, this, [=]() {
        if (ticketWindow) {
            ticketWindow->close();
            delete ticketWindow;
            ticketWindow = nullptr;
        }

        this->close();
        if (m_mainpage) m_mainpage->show();
    });

    topLayout->addWidget(btnBack);


    // 新按钮（并排显示）
    QPushButton *btnOther = new QPushButton("语音输入打印小票", this);
    btnOther->setStyleSheet("background:#FFC107; padding:10px; font-size:20px;border-radius: 10px;");
    btnOther->setFixedHeight(50);

    connect(btnOther, &QPushButton::clicked,
            this, &CartPage::onBtnOtherClicked);

    topLayout->addWidget(btnOther);

    // 把两个按钮加进整体布局
    rootLayout->addLayout(topLayout);


    // listWidget 固定高度（防止挤掉下面按钮）
//    ui->listWidget->setMinimumHeight(240);
//    ui->listWidget->setMaximumHeight(260);
    ui->listWidget->setMinimumHeight(295);
    ui->listWidget->setMaximumHeight(380);
    rootLayout->addWidget(ui->listWidget);

    // 显示总价
    totalLabel = new QLabel("总价：￥0", this);
    totalLabel->setStyleSheet("font-size:22px; font-weight:bold; padding:5px;color:white;");
    totalLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    rootLayout->addWidget(totalLabel);

    // 空白填充，让按钮永远贴底部
    rootLayout->addStretch();

    // 底部按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;

    btnClear = new QPushButton("清空购物车");
    btnClear->setStyleSheet("background:#AAAAAA; padding:10px; font-size:18px;border-radius: 10px;");
    btnLayout->addWidget(btnClear);

    btnPay = new QPushButton("立即结算");
    btnPay->setStyleSheet("background:#D50000; color:white; padding:10px; font-size:18px;border-radius: 10px;");
    btnLayout->addWidget(btnPay);

    rootLayout->addLayout(btnLayout);

    // 信号连接
    connect(btnClear, &QPushButton::clicked, this, &CartPage::clearCart);
    connect(btnPay, &QPushButton::clicked, this, &CartPage::pay);

    // 刷新信号
    if (m_manager) {
        connect(m_manager, &CartManager::cartUpdated,
                this, &CartPage::refreshList);
    }

    refreshList();

    // === 初始化 TCP 客户端 ===
    tcp = new QTcpSocket(this);

    // 自动连接服务器
    tcp->connectToHost("192.168.171.58", 8888);  // <-- 换成你服务器的 IP 和端口

    // 连接成功
    connect(tcp, &QTcpSocket::connected, this, [=](){
        QMessageBox::information(this, "TCP", "创建订单成功！");
    });

    // 断开连接
    connect(tcp, &QTcpSocket::disconnected, this, [=](){
        QMessageBox::warning(this, "TCP", "订单已完成，请尽快前往前台领取！");
    });

    // 错误处理
//    connect(tcp, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError){
//        QMessageBox::warning(this, "TCP 错误", tcp->errorString());
//    });

    this->setObjectName("cartpage");   // 必须设置对象名
    this->setStyleSheet("QMainWindow#cartpage { background-image: url(:/new/prefix1/background.jpg); }");

    if (m_led.init()==false) {
        qDebug() << "LED init fail";
    } else {
        m_led.ledOff(1);
        m_led.ledOff(2);
        m_led.ledOff(3);
        m_led.ledOff(4);
    }
}

CartPage::~CartPage()
{
    delete ui;
}

void CartPage::refreshList()
{
    if (!m_manager) return;

    ui->listWidget->clear();

    auto items = m_manager->getItems();
    int total = 0;

    for (const auto &it : items)
    {
        QString text = QString("%1 × %2 （%3元）")
            .arg(it.name)
            .arg(it.count)
            .arg(it.price * it.count);

        ui->listWidget->addItem(text);

        total += it.price * it.count;
    }

    // 更新总价
    totalLabel->setText(QString("总价：%1元").arg(total));
}

void CartPage::clearCart()
{
    if (!m_manager) return;
    m_manager->clear();
    refreshList();
}

void CartPage::pay()
{
    if (!m_manager) return;

    auto items = m_manager->getItems();
    if (items.isEmpty()) {
        QMessageBox::information(this, "提示", "购物车为空！");
        return;
    }

    // === 生成随机叫号 ===
    QChar letter = QChar('A' + (qrand() % 6));   // A~F
    int number = (qrand() % 999) + 1;            // 1~999
    QString callNumber = QString("%1%2")
            .arg(letter)
            .arg(number, 3, 10, QChar('0'));    // 例如：A023

    // === 小票显示到 listWidget（显示在最上面） ===
    ui->listWidget->insertItem(0, "取餐号：" + callNumber);
    ui->listWidget->insertItem(1, "订单生成成功！请稍候叫号…");

    // === 生成订单号 ===
    QString orderId = QString("NO.%1")
            .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));

    // === 生成小票 ===
    QString receipt;
    receipt += "\n\n\n\n";
    receipt += "————— 麦当劳自助点餐机 —————\n";
    receipt += QString("取餐号：%1\n").arg(callNumber);
    receipt += QString("订单号：%1\n").arg(orderId);
    receipt += "商品名          数量    小计\n";

    int total = 0;
    for (auto &it : items) {
        int sub = it.price * it.count;
        total += sub;

        receipt += QString("%1\t × %2\t %3元\n")
                       .arg(it.name, -8)
                       .arg(it.count)
                       .arg(sub);
    }

    receipt += "--------------------------------\n";
    receipt += QString("总价：%1 元\n").arg(total);
    receipt += "支付方式：微信支付\n";
    receipt += QString("时间：%1\n")
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    receipt += "--------------------------------\n";
    receipt += "         谢谢惠顾！\n";

    ticketWindow = new QWidget(this);
    ticketWindow->setWindowTitle("小票打印");
    ticketWindow->setFixedSize(320, 480);

    QVBoxLayout *layout = new QVBoxLayout(ticketWindow);
    QLabel *label = new QLabel(receipt, ticketWindow);
    label->setStyleSheet("font-family: monospace; font-size: 14px;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    ticketWindow->show();

    // === 发送小票到服务器 ===
    if (tcp->state() != QAbstractSocket::ConnectedState) {
        tcp->connectToHost("192.168.1.100", 8888); // 自动重连
        tcp->waitForConnected(1500);
    }

    if (tcp->state() == QAbstractSocket::ConnectedState) {
        tcp->write(receipt.toUtf8());
        tcp->flush();
    } else {
        QMessageBox::warning(this, "发送失败", "无法连接服务器，发送小票失败！");
    }



    // === 清空购物车 ===
    clearCart();

    // === 30 秒后自动返回 MainPage ===
//    QTimer::singleShot(30000, this, [=]() {
//        ticketWindow->close();          // 关掉小票窗口

//        if (m_mainpage) {
//            m_mainpage->show();   // 返回主页面
//        }
//        this->close();            // 关闭当前 cartpage
//    });

}

void CartPage::setMainPage(MainPage *mp)
{
        m_mainpage = mp;
}

void CartPage::playVoice()
{
    QTcpSocket sock;
    sock.connectToHost("192.168.171.60", 55555);

    if (!sock.waitForConnected(3000)) {
        qDebug() << "连接服务器失败！";
        return;
    }
    qDebug() << "连接成功！";

    // === 录音（调用 arecord） ===
    QProcess::execute("arecord -d3 -c1 -r16000 -twav -fS16_LE 1.wav");
    qDebug() << "录音完成";

    QFile file("1.wav");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开录音文件";
        return;
    }

    int file_size = file.size();
    qDebug() << "file_size =" << file_size;

    // 发送文件大小
    sock.write(reinterpret_cast<char*>(&file_size), 4);
    sock.flush();

    // 分段发送
    QByteArray buf;
    while (!file.atEnd()) {
        buf = file.read(1024);
        sock.write(buf);
        sock.flush();
    }
    qDebug() << "文件发送完毕";

    // 等待服务器回应
    if (!sock.waitForReadyRead(5000)) {
        qDebug() << "等待服务器返回超时！";
        return;
    }

    QByteArray recv = sock.readAll();
    QString result = QString::fromUtf8(recv);
    qDebug() << "服务器返回:" << result;

    // ---- 原逻辑 ----
    if (result.contains("相册")) {
        qDebug() << "play photo";
    }
    if (result.contains("音乐")) {
        qDebug() << "play music";
    }
    if (result.contains("视频")) {
        qDebug() << "play video";
    }
    if (result.contains("没有识别结果")) {
        qDebug() << "没有识别结果";
    }

    // ---- 新逻辑 ----
    if (result.contains("东北雨姐")) {
        qDebug() << "play 东北雨姐";
    }
    if (result.contains("小票")) {
        qDebug() << "print 小票";

        // ========= 显示弹窗 =========
        QDialog *dlg = new QDialog(this);
        dlg->setWindowTitle("提示");
        dlg->setFixedSize(260, 120);

        QLabel *label = new QLabel("小票打印中...", dlg);
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout(dlg);
        layout->addWidget(label);

        dlg->show();

        // ========= LED 爆闪（20 次）=========
        auto flashTimer = new QTimer(dlg);
        flashTimer->setInterval(100);   // 每次切换100ms ~ 你原来的50+50ms

        int *count = new int(0);

        connect(flashTimer, &QTimer::timeout, this, [=]() mutable {

            if ((*count) % 2 == 0)
            {
                m_led.ledOn(1);
                m_led.ledOn(2);
                m_led.ledOn(3);
                m_led.ledOn(4);
            }

            else
            {
                m_led.ledOff(1);
                m_led.ledOff(2);
                m_led.ledOff(3);
                m_led.ledOff(4);
            }


            (*count)++;

            if (*count >= 40) {  // 20次开关 = 40次变化
                flashTimer->stop();
                m_led.ledOff(1);
            }
        });

        flashTimer->start();

        // ========= 5 秒后显示“打印完成” =========
        QTimer::singleShot(5000, dlg, [=]() {
            label->setText("打印完成！");
        });

        // ========= 6 秒后关闭弹窗 =========
        QTimer::singleShot(6000, dlg, [=]() {
            dlg->close();
        });
    }
}

void CartPage::onBtnOtherClicked()
{
    qDebug() << "Ticket";
    playVoice();


       // m_led.ledOn(1);
//        m_led.closeDev();

}



