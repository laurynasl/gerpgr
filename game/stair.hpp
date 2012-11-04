#ifndef STAIR_HPP_
#define STAIR_HPP_

#include "mapobject.hpp"

class Stair : public MapObject {
    protected:
        QString mapName;
        int targetX;
        int targetY;
        bool up;
    public:
        Stair(QString mapName, int targetX, int targetY, bool isUp, int tile);
        virtual ~Stair();
        
        virtual QString getClassName() const;
        QString getMapName() const;
        int getTargetX() const;
        int getTargetY() const;
        bool isUp() const;
};

#endif
