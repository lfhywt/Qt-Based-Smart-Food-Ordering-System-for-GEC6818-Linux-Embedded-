#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpServer> //服务器
#include <QTcpSocket> //客户端
#include <QList> //链表的头文件   c++(list)->List->QList
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_listen_clicked();

    void on_btn_close_clicked();

    void on_btn_send_clicked();

    void onRecvNewClientSlot(); //新的客户端连接的槽函数
    void onRecvDataSlot(); //客户端发送数据过来的槽函数
        void on_btn_finish_clicked();

private:
    Ui::MainWindow *ui;

    //实例化一个服务器的对象
    QTcpServer *tcpServer;

    //如果只是一个客户端的对象 那么你只能和一个客户端聊天
    //QTcpSocket *clientSocket;  //实例化一个客户端的对象（和它通信）

    //存储多个客户端的对象(用链表来存储)
    QList<QTcpSocket *> clientList; //链表的名字clientList
    int orderCount = 1;   // 用来给订单编号



};
#endif // MAINWINDOW_H
