#include "page1.h"
#include "ui_page1.h"
#include "mainpage.h"
#include "cartpage.h"
#include "cartmanager.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <QToolButton>


Page1::Page1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Page1)
{
    ui->setupUi(this);

    cart = new CartManager(this);
    cartPage = new CartPage(cart);
    ui->scrollArea->setWidgetResizable(true);

    // 点餐图片点击 → 加入购物车
    connect(this, &Page1::addToCart, cart, &CartManager::addItem);

    // 小红点数量更新
    connect(cart, &CartManager::cartUpdated, this, &Page1::updateBadge);

    // 购物车按钮
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Page1::openCartPage);

    updateBadge();

    addScrollItem("清真猪肉堡", 18);
    addScrollItem("双层牛肉堡", 16);
    addScrollItem("雨姐豪华汉堡", 25);
    addScrollItem("雨姐同款红薯粉条", 22);
    addScrollItem("薯条", 12);
    addScrollItem("爆浆鸡排", 8);
    addScrollItem("可乐", 8);
    addScrollItem("雨姐的祝福", 66);
    addScrollItem("鸡哥手办", 11);

//    ui->scrollArea->viewport()->setStyleSheet("background-color: #D50000;");
//    ui->scrollAreaWidgetContents->setStyleSheet("background-color: #D50000;");
//    ui->scrollArea->setStyleSheet("border: none;");

    // ScrollArea 背景 = 黄色
    ui->scrollArea->viewport()->setStyleSheet("background-color: #FFCC00;");
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: #FFCC00;");
    ui->scrollArea->setStyleSheet("border: none;");

    ui->centralwidget->setStyleSheet("background-color: #FFCC00;");
  //  ui->pushButton_2->setStyleSheet("background-color: #2DBE60;");
    ui->badge->setStyleSheet("background:red; color:white; border-radius:10px; font-size:14px;");


    ui->pushButton_2->setStyleSheet(
           "background-color: #4CAF50;"
           "color: white;"
           "font-size: 18px;"
           "border: none;"
           "border-radius: 20px;"
           "padding: 8px 16px;"
       );
}


Page1::~Page1()
{
    delete ui;
}

void Page1::setMainPage(MainPage *p)
{
    m_mainPage = p;
    cartPage->setMainPage(m_mainPage);
}

void Page1::onItemClicked()
{
    emit addToCart("默认商品", 10);
}

void Page1::openCartPage()
{
    this->hide();
    cartPage->show();
    cartPage->raise();
}

void Page1::updateBadge()
{
    int c = cart->getTotalCount();

       if (c > 0) {
           ui->badge->setText(QString(" %1 ").arg(c));
           ui->badge->show();      // 显示
       } else {
           ui->badge->hide();      // 隐藏
       }
}


void Page1::addScrollItem(const QString &name, int price)
{
    // 确保 scrollArea 可自适应内容
    ui->scrollArea->setWidgetResizable(true);

    // 如果还没有布局，就创建一个 3 列的网格布局，放到 scrollAreaWidgetContents 上
    QGridLayout *grid = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!grid) {
        grid = new QGridLayout;
        grid->setSpacing(12);
        grid->setContentsMargins(8, 8, 8, 8);
        ui->scrollAreaWidgetContents->setLayout(grid);

        // 可选：确保 contents 的最小宽度能撑开每行 3 个按钮（防止被压扁）
        ui->scrollAreaWidgetContents->setMinimumWidth(3 * 220 + 40);
    }

    // 计算下一个插入位置 (row, col)
    int idx = grid->count();        // 当前已有的 item 数
    const int cols = 3;
    int row = idx / cols;
    int col = idx % cols;

    // 使用 QToolButton 显示图片 + 文字（文字在图下）
    QToolButton *btn = new QToolButton(ui->scrollAreaWidgetContents);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setFixedSize(220, 200);                // 适当大小，你可以按需改
    btn->setAutoRaise(false);

    // 尝试给按钮设置资源图片（按资源命名 p1.png,p2.png...）
    QString imgPath = QString(":/new/prefix1/p%1.png").arg(idx + 1);
    QIcon ico(imgPath);
    if (!ico.isNull()) {
        btn->setIcon(ico);
//        btn->setIconSize(QSize(180, 120));
        btn->setIconSize(QSize(140, 90));  // 推荐大小
    } else {
        // 如果没有图片可用，可以设置一个占位文本或样式
        btn->setStyleSheet("background:#f6f6f6;");
    }

    // 文字与价格
    btn->setText(name + "\n￥" + QString::number(price));
    btn->setProperty("price", price);
    btn->setProperty("name", name);

    // 点击连接：发出 addToCart(name, price)
    connect(btn, &QToolButton::clicked, this, [this, btn]() {
        QString n = btn->property("name").toString();
        int p = btn->property("price").toInt();
        emit addToCart(n, p);
        qDebug() << "加入购物车:" << n << p;
    });

    // 将按钮加到网格布局
    grid->addWidget(btn, row, col);
    btn->setStyleSheet(
        "QToolButton {"
        "   background-color: #D50000;"      /* 麦当劳红 */
        "   border-radius: 20px;"
        "   padding: 10px;"
        "   border: 2px solid #B00000;"      /* 深红边框，更有质感 */
        "   font-size: 16px;"
        "   color: white;"                   /* 白字 */
        "}"
        "QToolButton:hover {"
        "   background-color: #E00000;"      /* 悬停更亮一点 */
        "}"
    );




}

