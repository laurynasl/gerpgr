#include "QItemsChecklist.hpp"
#include <QLayoutItem>
#include <QCheckBox>

void QItemsChecklist:: clear() {
    while (!checkBoxes.isEmpty()) {
        QWidget* child = checkBoxes.takeFirst();
        layout->removeWidget(child);
        delete child;
    }
}

QItemsChecklist:: QItemsChecklist() {
    layout = new QVBoxLayout(this);
}

QItemsChecklist:: ~QItemsChecklist() {
    clear();
}

void QItemsChecklist:: setItemsList(ItemsList _items) {
    items = _items;
    clear();
    for (int i=0; i<items.count(); i++) {
        QCheckBox* checkBox = new QCheckBox(items[i].description());
        layout->addWidget(checkBox);
        checkBoxes.append(checkBox);
    }
}

QBitArray QItemsChecklist:: getSelection() {
    QBitArray array(checkBoxes.size());
    for (int i=0; i<checkBoxes.size(); i++) {
        array[i] = checkBoxes[i]->isChecked();
    }
    return array;
}
