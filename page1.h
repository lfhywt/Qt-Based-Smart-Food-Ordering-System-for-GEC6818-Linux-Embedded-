#ifndef PAGE1_H
#define PAGE1_H

#include <QMainWindow>

class MainPage;        // ★ 前置声明
class CartManager;
class CartPage;

namespace Ui {
class Page1;
}

class Page1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Page1(QWidget *parent = nullptr);
    ~Page1();

    void setMainPage(MainPage *p);   // ★ 必须有这个函数！
    void addScrollItem(const QString &name, int price);

private:
    Ui::Page1 *ui;

    MainPage *m_mainPage = nullptr;  // ★ 保存 mainpage
    CartManager *cart = nullptr;
    CartPage *cartPage = nullptr;

private slots:
    void onItemClicked();
    void openCartPage();
    void updateBadge();

signals:
    void addToCart(QString name, int price);
};

#endif // PAGE1_H
