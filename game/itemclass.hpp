#ifndef ITEM_CLASS_HPP_
#define ITEM_CLASS_HPP_

#include "attackdescriptor.hpp"
#include "../parser/parser.hpp"
#include <QString>
#include <QList>
#include <QSet>

const int NonStackable = 0;
const int Stackable = 1;
const int Depletable = 2;

/** \brief Descriptor class for Item
 */
class ItemClass {
    public:
        AttackDescriptor onehanded, twohanded, thrown;
        QSet<int> features;
        QMap<int,QString> stringFeatures;
        QMap<int,int> intFeatures;
        QMap<int,int> tokenFeatures;
        QString name;
        int stackability;
        
        ItemClass();
        ~ItemClass();
        ItemClass(const ItemClass& other);
        void load(Parser& parser);
        void print() const;
        
        QString getKeyValue() const;
        bool isPrototype() const;
        int getDv() const;
        int getEquipSlot() const;
};

#endif
