#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "masks.hpp"
#include <QString>

/** \brief Single item or a stack of stackable items
 */
class Item {
    protected:
        int _type;
        int _count;
    public:
        Item();
        Item(const int itemType, int _count = 1);
        Item(const Item& other);
        ~Item();
        const int type() const;
        const int count() const;
        const QString description() const;
        const QString name() const;
        const bool canStackWith(const Item& other) const;
        void increaseCount(int diff);
        const bool stackable() const;
        //bool canEquipAs(int equipSlot);
};

#endif
