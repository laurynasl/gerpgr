#ifndef ITEMS_LIST_HPP_
#define ITEMS_LIST_HPP_

#include <QList>
#include "item.hpp"

/** \brief Container and management class for Item
 */
class ItemsList {
    protected:
        QList<Item> _list;
    public:
        ItemsList();
        ItemsList(const ItemsList& other);
        ItemsList(QList<Item> list);
        const int count() const;
        const bool empty() const;
        //Item& last();
        const Item& last() const;
        //Item takeFirst();
        Item takeAllAt(int i);
        Item takeOneAt(int i);
        Item takeCountAt(int i, int count);
        void append(const Item& item);
        void clear();
        ItemsList& operator +=(const ItemsList& other);
        void append(const ItemsList& other);
        const Item& operator[] (int i) const;
};

#endif
