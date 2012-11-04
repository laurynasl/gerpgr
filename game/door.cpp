#include "door.hpp"
// #include "tables.hpp"

Door:: Door(bool closed, int closedTile, int openTile, bool locked, QString code) {
    this->closed = closed;
    this->closedTile = closedTile;
    this->openTile = openTile;
    this->locked = false;
    tile = closed ? closedTile : openTile;
    this->locked = locked;
    this->code = code;
}

Door:: ~Door() {
}

QString Door:: getClassName() const {
    return "door";
}

bool Door:: isObstacle() const {
    return isClosed();
}

bool Door:: isBulletproof() const {
    return isClosed();
}

bool Door:: isVisionproof() const {
    return isClosed();
}

bool Door:: isClosed() const {
    return closed;
}

bool Door:: open() {
    if (closed) {
        closed = false;
        tile = openTile;
        return true;
    }
    else {
        return false;
    }
}

bool Door:: close() {
    if (closed) {
        return false;
    }
    else {
        closed = true;
        tile = closedTile;
        return true;
    }
}

bool Door:: isLocked() const {
    return locked;
}

bool Door:: lock() {
    if (!locked) {
        locked = true;
        return true;
    }
    else {
        return false;
    }
}

bool Door:: unlock() {
    if (locked) {
        locked = false;
        return true;
    }
    else {
        return false;
    }
}

QString Door:: getCode() const {
    return code;
}
