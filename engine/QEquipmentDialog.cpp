#include "QEquipmentDialog.hpp"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QMessageBox>
#include "../game/tables.hpp"

const int equipmentNameAlignment = 15;

QEquipmentDialog:: QEquipmentDialog() {
    setCursor(Qt::PointingHandCursor);
    QVBoxLayout* layout = new QVBoxLayout(this);
    listWidget = new QListWidget(this);
    QFont courier("Courier", 12);
    listWidget->setFont(courier);
    layout->addWidget(listWidget);
    for (int i=0; i<equipmentTypesCount; i++) {
        listWidget->addItem("");
    }
    connect(listWidget, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onListClick(const QModelIndex&)));
}

void QEquipmentDialog:: exec1(Monster* monster) {
    this->monster = monster;
    for (int i=0; i<equipmentTypesCount; i++) {
        int equipSlot = i + equipmentTypeMask;
        QString text = Tables::tokenToName(equipSlot);
        text = text.leftJustified(equipmentNameAlignment, ' ');
        if (monster->equipment.contains(equipSlot)) {
            text += monster->equipment[equipSlot].description();
        }
        else {
            text += "None";
        }
        listWidget->item(i)->setText(text);
    }
    exec();
}

void QEquipmentDialog:: onListClick(const QModelIndex& index) {
    int row = index.row();
    int equipSlot = row+equipmentTypeMask;
    //cout << "row=" << row << endl;
    
    QPair<bool,QString> isEquippable = monster->isSlotEquippable(equipSlot);
    if (!isEquippable.first) {
        QMessageBox::information(this, "Cannot equip", isEquippable.second);
        return;
    }
    
    QListWidgetItem* item = listWidget->item(row);
    QString text = item->text();
    
    if (monster->equipment.contains(equipSlot)) {
        monster->equip(-1, equipSlot);
    }
    else {
        int selection = selectOneItemDialog.exec1(monster, equipSlot);
        //cout << "selected=" << selection << endl;
        monster->equip(selection, equipSlot);
    }
    text.truncate(equipmentNameAlignment);
    if (monster->equipment.contains(equipSlot)) {
        text += monster->equipment[equipSlot].description();
    }
    else {
        text += "None";
    }
    // call selection window/menu and select object
    item->setText(text);
}
