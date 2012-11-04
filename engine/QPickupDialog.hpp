#ifndef QPICKUP_DIALOG_
#define QPICKUP_DIALOG_

#include <QObject>
#include <QDialog>
#include <QBitArray>
#include "../game/item.hpp"
#include "../game/itemslist.hpp"
#include "QItemsChecklist.hpp"

class QPickupDialog : public QDialog{
    Q_OBJECT
    protected:
        QItemsChecklist* checklist;
    //    QList<Item> itemsList;
    public:
        QPickupDialog();
        QBitArray exec1(const ItemsList items);
};

#endif
