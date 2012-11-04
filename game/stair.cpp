#include "stair.hpp"

Stair:: Stair(QString mapName, int targetX, int targetY, bool up, int tile) {
    this->mapName = mapName;
    this->targetX = targetX;
    this->targetY = targetY;
    this->up = up;
    this->tile = tile;
    cout << "creating Stair with tile " << tile << endl;
}

Stair:: ~Stair() {
}

QString Stair:: getClassName() const {
    return "stair";
}

QString Stair:: getMapName() const {
    return mapName;
}

int Stair:: getTargetX() const {
    return targetX;
}

int Stair:: getTargetY() const {
    return targetY;
}

bool Stair:: isUp() const {
    return up;
}
