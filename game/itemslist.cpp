#include "itemslist.hpp"
#include <assert.h>
#include <iostream>

using namespace std;

ItemsList:: ItemsList() {
}

ItemsList:: ItemsList(const ItemsList& other) {
    _list = other._list;
}

ItemsList:: ItemsList(QList<Item> list) {
    _list = list;
}

const int ItemsList:: count() const {
    return _list.count();
}

const bool ItemsList:: empty() const {
    return _list.empty();
}

const Item& ItemsList:: last() const {
    return _list.last();
}

// Item ItemsList:: takeFirst() {
//     return _list.takeFirst();
// }

Item ItemsList:: takeAllAt(int i) {
    assert(i >= 0);
    assert(i < _list.count());
    return _list.takeAt(i);
}

Item ItemsList:: takeOneAt(int i) {
    assert(i >= 0);
    assert(i < _list.count());
    if ((_list[i].count() > 1) && (_list[i].stackable())) {
        return takeCountAt(i, 1);
    }
    else {
        return takeAllAt(i);
    }
}

Item ItemsList::  takeCountAt(int i, int count) {
    assert(i >= 0);
    assert(i < _list.count());
    assert(count > 0);
    if ((count >= _list[i].count()) || !_list[i].stackable()) {
        return takeAllAt(i);
    }
    Item item(_list[i].type(), count);
    _list[i].increaseCount(-count);
    return item;
}

void ItemsList:: append(const Item& item) {
    if (!empty()) {
        for (int i=0; i<count(); i++) {
            if (_list[i].canStackWith(item)) {
                _list[i].increaseCount(item.count());
                return;
            }
        }
    }
    _list.append(item);
}

void ItemsList:: clear() {
    _list.clear();
}

ItemsList& ItemsList:: operator +=(const ItemsList& other) {
    append(other);
    return *this;
}

void ItemsList:: append(const ItemsList& other) {
    if (other.empty()) return;
    for (int i=0; i<other.count(); i++) {
        append(other[i]);
    }
}

const Item& ItemsList:: operator[] (int i) const {
    assert(i >= 0);
    assert(i < _list.count());
    return _list[i];
}
