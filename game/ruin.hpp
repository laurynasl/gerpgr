#ifndef RUIN_HPP_
#define RUIN_HPP_

#include <QString>

enum RuinType {
    InvalidRuin = -1,
    Dungeon,
    InfiniteDungeon,
};

class Ruin {
    public:
        QString name;
        int x;
        int y;
        RuinType type;
        
        Ruin();
        Ruin(QString name, int x, int y, RuinType type=Dungeon);
        Ruin(const Ruin& other);
};

#endif
