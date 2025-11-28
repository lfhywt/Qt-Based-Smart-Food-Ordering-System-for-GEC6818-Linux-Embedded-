#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QTextEdit>
#include "easyInput/easyinput.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
//    bool eventFilter(QObject *watched, QEvent *event) override;   // ✔ 正确写法

private:
    void mainWindowInit();   // ✔ 普通函数
    void saveSettings();
    void loadSettings();

private slots:
    void on_btn_login_clicked();   // ✔ 槽函数
   // void slotHideInput();          // ✔ 槽函数

private:
    Ui::MainWindow *ui;

    easyInput *eInput;      // ✔ 放在 private
    bool inputFlag;         // ✔ 放在 private
};

#endif // MAINWINDOW_H
