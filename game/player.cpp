#include "player.hpp"
#include "masks.hpp"
#include <fstream>

void Player:: load(Parser &parser) {
    parser >> name >> mapName;
    pLoad(parser);
}

void Player:: save(ofstream& file) const {
    file << "player \"" << name.toStdString() << "\" \"";
    file << mapName.toStdString() << "\" " << x << " " << y << endl;
    pSave(file, "    ");
    file << "end" << endl << endl;
}

bool Player:: teleport(const QString& mapName, int x, int y) {
    bool result = Monster::teleport(mapName, x, y);
    emit playerMoved();
    return result;
}

void Player:: moveBy(int dx, int dy) {
    Monster::moveBy(dx, dy);
    emit playerMoved();
}

bool Player:: isMonster() {
    return false;
}

void Player:: random(int monsterType) {
    Monster::random(monsterType);
    emit healthChange();
}

void Player:: resetMaximums() {
    Monster::resetMaximums();
    emit healthChange();
}

void Player:: naturallyRegenerate() {
    Monster::naturallyRegenerate();
    emit healthChange();
}

void Player:: damaged() {
    Monster::damaged();
    emit healthChange();
}
