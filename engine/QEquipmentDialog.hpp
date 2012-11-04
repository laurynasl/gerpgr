#ifndef QEQUIPMENT_DIALOG_
#define QEQUIPMENT_DIALOG_

#include <QObject>
#include <QDialog>
#include <QListWidget>
#include <QModelIndex>
#include "../game/monster.hpp"
#include "QSelectOneItemDialog.hpp"

class QEquipmentDialog : public QDialog {
    Q_OBJECT
    protected:
        Monster* monster;
        QListWidget* listWidget;
        QSelectOneItemDialog selectOneItemDialog;
    public:
        QEquipmentDialog();
        void exec1(Monster* monster);
    public slots:
        void onListClick(const QModelIndex & index);
};

#endif
