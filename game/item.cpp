#include "item.hpp"
#include "tables.hpp"
#include <iostream>

using namespace std;

Item:: Item() {
}

Item:: Item(const int _type, int _count) {
    this->_type = _type;
    this->_count = _count;
    // ensure item is not prototype
    if (Tables::itemToClass(_type).isPrototype()) {
        throw QString("item.cpp: Prototype item is being constructed");
    }
}

Item:: Item(const Item& other) {
    _type = other._type;
    _count = other._count;
}

Item:: ~Item() {
}

const int Item:: type() const {
    return _type;
}

const int Item:: count() const {
    return _count;
}

const QString Item:: description() const {
    QString desc = name();
    switch (Tables::itemToClass(_type).stackability) {
        case Stackable: {
            if (_count > 1) {
                desc = QString::number(_count) + ' ' + desc + "(s)";
            }
            break;
        }
        case Depletable: {
            desc = desc + " (" + QString::number(_count) + " uses)";
            break;
        }
    }
    return desc;
}

const QString Item:: name() const {
    QString n = Tables::itemToClass(_type).name;
    return n;
}

const bool Item:: canStackWith(const Item& other) const {
    if (_type != other._type) {
        return false;
    }
    return stackable();
}

void Item:: increaseCount(int diff) {
    _count += diff;
}

const bool Item:: stackable() const {
    return Tables::itemToClass(_type).stackability == Stackable;
}

// bool Item:: canEquipAs(int equipSlot) {
//     return true; //TODO: write more
// }
