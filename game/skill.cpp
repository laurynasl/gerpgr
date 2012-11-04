#include "skill.hpp"

Skill:: Skill() {
    xp = 0;
}

Skill:: Skill(float xp) {
    this->xp = xp;
}

Skill:: Skill(const Skill& other) {
    xp = other.xp;
}

int Skill:: operator () () const {
    return (int)xp;
}

bool Skill:: increase(float value) {
    int lastLevel = (int)xp;
    xp += value / (100 * (lastLevel+1));
    int newLevel = (int) xp;
    if (newLevel > lastLevel+1) {
        xp = lastLevel + 1;
        return true;
    }
    else {
        return false;
    }
}
