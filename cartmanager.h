#ifndef CARTMANAGER_H
#define CARTMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>

class CartManager : public QObject
{
    Q_OBJECT

public:
    explicit CartManager(QObject *parent = nullptr);

    struct Item {
        QString name;
        int price;
        int count;
    };

    void addItem(const QString &name, int price);
    QVector<Item> getItems() const;
    void clear();
    int getTotalCount() const;


signals:
    void cartUpdated();  // 通知界面刷新

private:
    QVector<Item> m_items;
};

#endif // CARTMANAGER_H
