#include "triggerquestion.hpp"
#include "actionrandom.hpp"
#include <QString>
#include <QMessageBox>

TriggerQuestion:: TriggerQuestion(){
}

TriggerQuestion:: ~TriggerQuestion() {
}

void TriggerQuestion:: custLoad(Parser& parser) {
    parser >> message;
}

void TriggerQuestion:: custSave(ofstream& file) const {
    file << "\"" << message.toStdString() << "\"" << endl;
}

bool TriggerQuestion:: canActivate(Monster& monster) {
    if (monster.isMonster()) return false;
    return (QMessageBox::question(0, "Question", message, QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes);
}

QString TriggerQuestion:: name() const {
    return "question";
}
