#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>

class MainWindow;   // ★ 前置声明，不产生循环依赖
class Page1;

namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setMainWindow(MainWindow *mw);  // ★ 你需要的函数

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainPage *ui;
    MainWindow *m_mainwindow = nullptr;  // ★ 保存 MainWindow 指针
};

#endif // MAINPAGE_H
