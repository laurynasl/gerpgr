#include "faction.hpp"


Faction:: Faction() {
}

Faction:: ~Faction() {
}

void Faction:: load(Parser &parser) {
    throw QString("Faction::load() not implemented");
}

void Faction:: save(ofstream& file) {
    throw QString("Faction::save() not implemented");
}
