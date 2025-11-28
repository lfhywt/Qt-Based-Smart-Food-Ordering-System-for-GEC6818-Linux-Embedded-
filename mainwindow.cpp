#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainpage.h"
#include <QSettings>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

      loadSettings();
      mainWindowInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainWindowInit()
 {
     ui->centralwidget->installEventFilter(this);

     eInput = new easyInput(this);      //输入法实例化
     eInput->installEventFilter(this);  //安装事件过滤器
     inputFlag = true;
     ui->centralwidget->setFocus();   // 界面初始化时，先把焦点在menu处，否则输入法会先出现
 }


void MainWindow::on_btn_login_clicked()
{
//    MainPage *mainpage = new MainPage();
//    mainpage->setAttribute(Qt::WA_DeleteOnClose);
//     mainpage->setMainWindow(this);
//    this->setStyleSheet("");
//    this->hide();
//    mainpage->show();
    QString account = ui->lineEdit_account->text().trimmed();
        QString password = ui->lineEdit_password->text().trimmed();

        // 简单校验（实际可以接数据库/网络）
        if(account == "admin" && password == "123456")
        {
            saveSettings();   // 保存记住密码、自动登录选项

                MainPage *mainpage = new MainPage();
                mainpage->setAttribute(Qt::WA_DeleteOnClose);
                 mainpage->setMainWindow(this);
                this->hide();
                mainpage->show();
        }
        else
        {
            QMessageBox::warning(this, "错误", "账号或密码不正确！");
        }
}

void MainWindow::saveSettings()
{
    QSettings setting("MyApp", "LoginApp");

    setting.setValue("account", ui->lineEdit_account->text());
    setting.setValue("remember", ui->checkBox_2->isChecked());
    setting.setValue("autologin", ui->checkBox->isChecked());

    if(ui->checkBox_2->isChecked())
    {
        setting.setValue("password", ui->lineEdit_password->text());
    }
    else
    {
        setting.setValue("password", "");
    }
}

void MainWindow::loadSettings()
{
    QSettings setting("MyApp", "LoginApp");

    QString account   = setting.value("account").toString();
    QString password  = setting.value("password").toString();
    bool remember     = setting.value("remember").toBool();
    bool autologin    = setting.value("autologin").toBool();

    ui->lineEdit_account->setText(account);
    ui->checkBox_2->setChecked(remember);
    ui->checkBox->setChecked(autologin);

    if(remember)
        ui->lineEdit_password->setText(password);

    // 自动登录
    if(autologin)
    {
        QTimer::singleShot(100, this, &MainWindow::on_btn_login_clicked);
    }
}
