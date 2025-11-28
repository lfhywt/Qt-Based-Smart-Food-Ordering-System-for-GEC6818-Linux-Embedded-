#ifndef CARTPAGE_H
#define CARTPAGE_H

#include <QMainWindow>
#include "ui_cartpage.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QTimer>
#include "mainpage.h"
#include <QTcpSocket>

#include <QFile>
#include <QDebug>
#include <QProcess>
#include "ledqt.h"

class CartManager;

class CartPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit CartPage(CartManager *manager, QWidget *parent = nullptr);
    ~CartPage();

    void refreshList();
    void pay();
        void clearCart();
        void setMainPage(MainPage *mp);
           void onBtnOtherClicked();
           void playVoice();

private:
    Ui::cartpage *ui;   // !!! 必须小写，和 UI 文件一致 !
    CartManager *m_manager;
    QLabel *totalLabel;     // 显示总价
      QPushButton *btnClear;  // 清空按钮
      QPushButton *btnPay;    // 结算按钮
       MainPage *m_mainpage = nullptr;
        QWidget *ticketWindow = nullptr;
        QTcpSocket *tcp;
         ledqt m_led;   // 成员对象
};

#endif // CARTPAGE_H
