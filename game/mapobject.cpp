#include "mapobject.hpp"

int MapObject:: getTile() const {
    return tile;
}

bool MapObject:: isObstacle() const {
    return false;
}

bool MapObject:: isBulletproof() const {
    return false;
}

bool MapObject:: isVisionproof() const {
    return false;
}
