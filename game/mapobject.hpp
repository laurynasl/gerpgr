#ifndef ABSTRACT_MAP_OBJECT_HPP_
#define ABSTRACT_MAP_OBJECT_HPP_

#include <QString>
#include <iostream>

using namespace std;

class MapObject {
    protected:
        int tile;
    public:
        virtual QString getClassName() const = 0;
        int getTile() const;
        virtual bool isObstacle() const;
        virtual bool isBulletproof() const;
        virtual bool isVisionproof() const;
};

#endif
