#include "triggerplayer.hpp"


void TriggerPlayer:: custLoad(Parser& parser) {
}

void TriggerPlayer:: custSave(ofstream& file) const {
    file << endl;
}

TriggerPlayer:: TriggerPlayer() {
}

TriggerPlayer:: ~TriggerPlayer() {
}

bool TriggerPlayer:: canActivate(Monster& monster) {
    return !monster.isMonster();
}

QString TriggerPlayer:: name() const {
    return "player";
}
