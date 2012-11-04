#include "ruin.hpp"


Ruin:: Ruin() {
    x = -1;
    y = -1;
    type = InvalidRuin;
}

Ruin:: Ruin(QString name, int x, int y, RuinType type) {
    this->name = name;
    this->x = x;
    this->y = y;
    this->type = type;
}

Ruin:: Ruin(const Ruin& other) {
    name = other.name;
    x = other.x;
    y = other.y;
    type = other.type;
}
