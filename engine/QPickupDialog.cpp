#include "QPickupDialog.hpp"
#include <QVBoxLayout>
#include <QPushButton>

QPickupDialog:: QPickupDialog() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    checklist = new QItemsChecklist();
    layout->addWidget(checklist);
    QPushButton* buttonOK = new QPushButton("OK", this);
    layout->addWidget(buttonOK);
    connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
}

QBitArray QPickupDialog:: exec1(const ItemsList items) {
    checklist->setItemsList(items);
    if (exec()) {
        // get selection from QItemsChecklist
        return checklist->getSelection();
    }
    else {
        QBitArray selection(items.count());
        return selection;
    }
}
