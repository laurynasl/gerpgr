#ifndef ENGINEINTERFACE_HPP_
#define ENGINEINTERFACE_HPP_

#include "gamemap.hpp"
#include "monster.hpp"
#include "item.hpp"
#include "itemslist.hpp"
#include <Qt>
#include <QBitArray>
#include <QString>

/// Abstract class for all engines
class EngineInterface {
    public:
        virtual void changeMap(GameMap *map) = 0;
        virtual void monsterMoved(Monster &monster, int dx, int dy) = 0;
        virtual void monsterMovedTo(Monster &monster, int oldX, int oldY) = 0;
        virtual void repaintStats(Monster &monster) = 0;
        virtual void updateSquare(int x, int y) = 0;
        virtual QBitArray getPickupSelection(const ItemsList items) = 0;
        virtual void manageEquipment(Monster* monster) = 0;
        virtual bool askPlayerYesNo(const QString& caption, const QString& text)= 0;
        virtual void focus(int x, int y) = 0;
};

#endif
