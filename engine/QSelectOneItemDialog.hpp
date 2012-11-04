#ifndef QSELECT_ONE_ITEM_DIALOG_HPP_
#define QSELECT_ONE_ITEM_DIALOG_HPP_

#include <Qt>
#include <QDialog>
#include <QList>
#include <QListWidget>
#include <QModelIndex>
#include "../game/monster.hpp"

class QSelectOneItemDialog : public QDialog {
    Q_OBJECT
    protected:
        Monster* monster;
        int equipmentSlot;
        QListWidget* listWidget;
        QList<int> inventoryIndexes;
        int row;
    public:
        QSelectOneItemDialog();
        int exec1(Monster* monster, int equipmentSlot);
    public slots:
        void onListClick(const QModelIndex & index);
};

#endif
