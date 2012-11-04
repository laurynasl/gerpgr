#ifndef QITEMC_CHECKLIST_HPP_
#define QITEMC_CHECKLIST_HPP_

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QBitArray>
#include <QCheckBox>
#include "../game/item.hpp"
#include "../game/itemslist.hpp"

class QItemsChecklist : public QWidget{
    Q_OBJECT
    protected:
        ItemsList items;
        QVBoxLayout* layout;
        QList<QCheckBox*> checkBoxes;
        void clear();
    public:
        QItemsChecklist();
        ~QItemsChecklist();
        void setItemsList(ItemsList _items);
        QBitArray getSelection();
};

#endif
