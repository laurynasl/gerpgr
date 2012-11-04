#ifndef DOOR_HPP_
#define DOOR_HPP_

#include "mapobject.hpp"

class Door : public MapObject {
    protected:
        bool closed;
        int closedTile;
        int openTile;
        bool locked;
        QString code; /// lock code
    public:
        Door(bool closed, int closedTile, int openTile, bool locked, QString code);
        virtual ~Door();
        
        virtual QString getClassName() const;
        virtual bool isObstacle() const;
        virtual bool isBulletproof() const;
        virtual bool isVisionproof() const;
        
        bool isClosed() const;
        bool open();
        bool close();
        
        bool isLocked() const;
        bool lock();
        bool unlock();
        QString getCode() const;
};

#endif
