#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>

/*
 * 并发服务器的模型
    1.有新的客户端连接 会触发信号
    2.在对应的客户单的槽函数中；如果有客户端发来数据 会触发信号
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 高级美化 — 仅视觉，不改逻辑
    // 设置窗口基础
    setWindowTitle("智能订单管理中心");
    setMinimumSize(1100, 700);

    // 统一字体（适配 Windows / Linux）
    QFont baseFont("Segoe UI", 10);
    ui->listWidget->setFont(baseFont);
    ui->textBrowser->setFont(QFont("Consolas", 11));
    ui->lineEdit_ip->setFont(baseFont);
    ui->lineEdit_port->setFont(baseFont);
    ui->btn_listen->setFont(QFont("Segoe UI", 11, QFont::DemiBold));
    ui->btn_close->setFont(QFont("Segoe UI", 11, QFont::DemiBold));
  //  ui->btn_send->setFont(QFont("Segoe UI", 11, QFont::DemiBold));
    ui->btn_finish->setFont(QFont("Segoe UI", 11, QFont::DemiBold));

    // 精致 QSS（暗黑高端风格）
    QString qss = R"(
    QMainWindow {
        background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #0b1220, stop:1 #071122);
    }
    QLabel {
        color: #e6f0fa;
    }
    QListWidget {
        background: rgba(255,255,255,0.03);
        color: #e6f0fa;
        border-radius: 10px;
        padding: 8px;
        border: 1px solid rgba(255,255,255,0.04);
    }
    QListWidget::item {
        padding: 12px 8px;
        margin: 4px 2px;
        border-radius: 8px;
    }
    QListWidget::item:selected {
        background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 rgba(40,120,255,0.18), stop:1 rgba(120,200,255,0.10));
        color: #ffffff;
    }
    QTextBrowser, QPlainTextEdit {
        background: rgba(15,20,28,0.9);
        color: #dff6ff;
        border-radius: 10px;
        border: 1px solid rgba(255,255,255,0.03);
        padding: 10px;
        font-family: Consolas, monospace;
    }
    QLineEdit {
        background: rgba(255,255,255,0.02);
        border: 1px solid rgba(255,255,255,0.04);
        color: #dfeef7;
        padding: 7px;
        border-radius: 6px;
    }
    QPushButton {
        background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2fb36a, stop:1 #1f8f4f);
        color: white;
        border-radius: 8px;
        padding: 8px 14px;
        font-weight: 600;
        min-height: 36px;
    }
    QPushButton#btn_close {
        background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #f05b5b, stop:1 #d13b3b);
    }
    QPushButton:disabled {
        background: rgba(255,255,255,0.03);
        color: rgba(255,255,255,0.5);
    }
    QGroupBox {
        color: #dbeffd;
        border: 1px solid rgba(255,255,255,0.03);
        border-radius: 8px;
        margin-top: 8px;
        padding: 8px;
    }
    QToolTip {
        background: #0b1220;
        color: #e6f0fa;
        border: 1px solid rgba(255,255,255,0.04);
    }
    )";

    this->setStyleSheet(qss);

    // 阴影效果（让关键面板浮起来）
    auto *shadowList = new QGraphicsDropShadowEffect(this);
    shadowList->setBlurRadius(28);
    shadowList->setOffset(0, 8);
    shadowList->setColor(QColor(0, 0, 0, 160));
    ui->listWidget->setGraphicsEffect(shadowList);

    auto *shadowBrowser = new QGraphicsDropShadowEffect(this);
    shadowBrowser->setBlurRadius(30);
    shadowBrowser->setOffset(0, 8);
    shadowBrowser->setColor(QColor(0, 0, 0, 170));
    ui->textBrowser->setGraphicsEffect(shadowBrowser);

    // 美化按钮形态（圆角、hover 用 qss 已处理），增加提示文本
    ui->btn_listen->setToolTip("启动服务器（绑定 IP:Port）");
    ui->btn_close->setToolTip("断开服务（关闭监听）");
    //ui->btn_send->setToolTip("向选中客户端发送文本（请从列表选择）");
    ui->btn_finish->setToolTip("将选中订单标记完成并关闭对应客户端连接");

    // 微调控件最小尺寸与边距（保持结构不变）
    ui->listWidget->setMinimumWidth(320);
    ui->textBrowser->setMinimumHeight(300);
    ui->lineEdit_ip->setMaximumWidth(200);
    ui->lineEdit_port->setMaximumWidth(120);
    ui->btn_listen->setMinimumWidth(110);
    ui->btn_close->setMinimumWidth(110);
    //ui->btn_send->setMinimumWidth(110);
    ui->btn_finish->setMinimumHeight(44);

    // 状态栏显示（更专业）
    if (statusBar()) {
        statusBar()->setStyleSheet("color:#9ed6ff;");
        statusBar()->showMessage("就绪 · 等待客户端连接");
    }



    /*
        服务器接收客户端的链接和接收客户端的数据都是通过:信号+槽函数机制 (不是用共有方法)
    */
    //初始化
    tcpServer = new QTcpServer;

    //3、当有新的客户端连接上来的时候,服务器类的对象tcpServer会 自动发射newConnection信号，关联槽函数
    connect(tcpServer,&QTcpServer::newConnection,this,&MainWindow::onRecvNewClientSlot);

//    connect(ui->btn_finish, &QPushButton::clicked,
//            this, &MainWindow::on_btn_finish_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//启动服务器
void MainWindow::on_btn_listen_clicked()
{
    ui->btn_listen->setEnabled(false);
    ui->btn_close->setEnabled(true);


    //2、绑定和监听
    QString ip = ui->lineEdit_ip->text();
    QString port = ui->lineEdit_port->text();

    tcpServer->listen(QHostAddress(ip),port.toUInt());

    ui->btn_listen->setEnabled(false);
}

//断开服务器
void MainWindow::on_btn_close_clicked()
{
    ui->btn_listen->setEnabled(true);
    ui->btn_close->setEnabled(false);

    tcpServer->close();
}

//发送数据
void MainWindow::on_btn_send_clicked()
{
#if 0 //单个客户端
    //给客户端对象发送数据
    QString data = ui->textEdit->toPlainText();
    //给客户端发数据
    clientSocket->write(data.toLocal8Bit());
#endif
#if 0
    //从左边的那个列表里面选中某一个客户端发送
    //1.找出列表的条目数---->确定链表里面元素的位置；比如第0个条目---->clientList.at(0)
    int curClient = ui->listWidget->currentRow();//你点击那个客户端在列表控件上的编号
    //2.找到客户端的对象
    QTcpSocket *client = clientList.at(curClient);
    //QTcpSocket *client = clientList[currentRow];
    //3.给选中的客户端发送数据
    client->write(ui->textEdit->toPlainText().toLocal8Bit());
#endif
}

//有新的客户端连接----->C语言里面的accept函数
void MainWindow::onRecvNewClientSlot()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    if (!clientSocket) return;

    // 保存 socket 到列表
    clientList.push_back(clientSocket);

    // 创建一个 QListWidgetItem，并把 socket 指针存到 UserRole 中
    // 将指针按整数存储（安全且兼容）
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(QString("订单 %1").arg(clientList.size()));
    // 存指针：把指针转换为 quintptr 存进去
    item->setData(Qt::UserRole, QVariant::fromValue((qulonglong)reinterpret_cast<quintptr>(clientSocket)));

    ui->listWidget->addItem(item);

    // 关联 readyRead
    connect(clientSocket, &QTcpSocket::readyRead,
            this, &MainWindow::onRecvDataSlot);

    // 建议：当客户端断开时也把它从列表移除（可选）
    connect(clientSocket, &QTcpSocket::disconnected, this, [=](){
        // 在断开回调里尝试移除对应的 list 项（如果还在）
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            QListWidgetItem *it = ui->listWidget->item(i);
            qulonglong val = it->data(Qt::UserRole).toULongLong();
            if (reinterpret_cast<quintptr>(clientSocket) == static_cast<quintptr>(val)) {
                delete ui->listWidget->takeItem(i);
                clientList.removeAt(i);
                break;
            }
        }
        clientSocket->deleteLater();
        // 重新编号剩余订单
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            ui->listWidget->item(i)->setText(QString("订单 %1").arg(i + 1));
        }
    });
}


//接收数据（自动触发的槽函数）
void MainWindow::onRecvDataSlot()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray data = client->readAll();

    QString text = QString::fromUtf8(data);

    // 每收到一条订单，编号自动 +1
    // 显示格式：
    // 订单1：
    // xxxxxxxx
    ui->textBrowser->append(
        QString("订单%1：\n%2\n------------------------------")
            .arg(orderCount)
            .arg(text)
    );
    orderCount++;
}


void MainWindow::on_btn_finish_clicked()
{
    int row = ui->listWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个订单！");
        return;
    }

    // 取出 item 并从 item 的 UserRole 中读取 socket 指针
    QListWidgetItem *item = ui->listWidget->item(row);
    if (!item) return;

    qulonglong stored = item->data(Qt::UserRole).toULongLong();
    QTcpSocket *sock = reinterpret_cast<QTcpSocket*>(static_cast<quintptr>(stored));

    // 1) 删除列表 UI 项
    delete ui->listWidget->takeItem(row);

    // 2) 在 clientList 中找到对应的 socket 并移除（按指针匹配）
    for (int i = 0; i < clientList.size(); ++i) {
        if (clientList.at(i) == sock) {
            // 先关闭 socket，再从容器移除（并 deleteLater）
            if (sock) {
                sock->disconnectFromHost();
                sock->close();
                sock->deleteLater();
            }
            clientList.removeAt(i);
            break;
        }
    }

    // 3) 重编号剩余订单，保证显示连续且和 clientList 索引对应
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        ui->listWidget->item(i)->setText(QString("订单 %1").arg(i + 1));
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("完成订单");
    msgBox.setText("该订单已完成并移除！");
    msgBox.setStyleSheet("QLabel{ color: black; }");  // 字体改成黑色
    msgBox.exec();
}
