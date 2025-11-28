#include "mainpage.h"
#include "ui_mainpage.h"

#include "page1.h"
#include "mainwindow.h"
#include <QDebug>

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    qDebug() << "MainPage objectName =" << this->objectName();
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::setMainWindow(MainWindow *mw)
{
    m_mainwindow = mw;
}

// 返回主界面（按钮3）
void MainPage::on_pushButton_3_clicked()
{
//    if (m_mainwindow) {
//        m_mainwindow->show();
//    }
//    this->close();
    this->hide();
    if (m_mainwindow)
        m_mainwindow->show();
}

// 跳转到 Page1（订单页面）
void MainPage::on_pushButton_clicked()
{
    Page1 *pageOrder = new Page1();
    pageOrder->setAttribute(Qt::WA_DeleteOnClose);

    pageOrder->setMainPage(this);  // ★ 你原本的逻辑保持不变

    this->hide();
    pageOrder->show();
}

void MainPage::on_pushButton_2_clicked()
{
    Page1 *pageOrder = new Page1();
    pageOrder->setAttribute(Qt::WA_DeleteOnClose);

    pageOrder->setMainPage(this);

    this->hide();
    pageOrder->show();
}
