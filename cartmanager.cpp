#include "cartmanager.h"

CartManager::CartManager(QObject *parent)
    : QObject(parent)
{
}

void CartManager::addItem(const QString &name, int price)
{
    // 如果已存在同名商品 → 数量+1
    for (auto &i : m_items) {
        if (i.name == name) {
            i.count += 1;
            emit cartUpdated();
            return;
        }
    }

    // 否则新增
    Item it;
    it.name = name;
    it.price = price;
    it.count = 1;

    m_items.push_back(it);
    emit cartUpdated();
}

QVector<CartManager::Item> CartManager::getItems() const
{
    return m_items;
}

void CartManager::clear()
{
    m_items.clear();
    emit cartUpdated();
}

int CartManager::getTotalCount() const
{
    int sum = 0;
    for (const auto &i : m_items) {
        sum += i.count;
    }
    return sum;
}

