#include "monsters.hpp"

Monsters* Monsters::instance = 0;

Monsters:: Monsters() {
}

Monsters:: ~Monsters() {
    //cout << "Monster::~Monster() before clear()" << endl;
    clear();
    //cout << "Monster::~Monster() after clear()" << endl;
}

Monsters& Monsters:: getInstance() {
    if (!instance) {
        instance = new Monsters();
        //instance->init();
    }
    return *instance;
}

void Monsters:: add(Monster* monster) {
    getInstance().monsters.insert(monster);
    //cout << "added " << monster->name.toStdString() << endl;
}

bool Monsters:: remove(Monster* monster) {
    return getInstance().monsters.remove(monster);
}

void Monsters:: clear() {
//     QSet<Monster*>& monsters = getInstance().monsters;
//     foreach (Monster* monster, monsters) {
//         if (monster->isMonster()) {
//             monster->print();
//             delete monster;
//         }
//     }
    getInstance().monsters.clear();
}

QSet<Monster*>& Monsters:: getAllMonsters() {
    return getInstance().monsters;
}
