#include <assert.h>
#include <QVBoxLayout>
#include "QSelectOneItemDialog.hpp"

QSelectOneItemDialog:: QSelectOneItemDialog() {
    setCursor(Qt::PointingHandCursor);
    QVBoxLayout* layout = new QVBoxLayout(this);
    listWidget = new QListWidget(this);
    //QFont courier("Courier", 12);
    //listWidget->setFont(courier);
    layout->addWidget(listWidget);
    assert(connect(listWidget, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onListClick(const QModelIndex&))));
}

int QSelectOneItemDialog:: exec1(Monster* monster, int equipmentSlot) {
    assert(monster != 0); //programming error prevention
    this->monster = monster;
    this->equipmentSlot = equipmentSlot;
    inventoryIndexes.clear();
    listWidget->clear();
    if (!monster->items.empty()) {
        for (int i=0; i<monster->items.count(); i++) {
            if (monster->canEquipAs(i, equipmentSlot)) {
                listWidget->addItem(monster->items[i].description());
                inventoryIndexes.append(i);
            }
        }
    }
    
    if (exec()) {
        return inventoryIndexes[row];
    }
    else {
        return -1;
    }
}

void QSelectOneItemDialog:: onListClick(const QModelIndex& index) {
    row = index.row();
    accept();
}
