#ifndef GAMEMAP_HPP_
#define GAMEMAP_HPP_

#include "testable.hpp"
#include "masks.hpp"
#include "monster.hpp"
#include "../parser/parser.hpp"
#include "monsterclass.hpp"
#include "item.hpp"
#include "itemslist.hpp"
#include "coordinateslist.hpp"
#include "trigger.hpp"
#include "abstractaction.hpp"
#include "world.hpp"
#include <fstream>
#include <Qt>
#include <QSet>
#include <QList>
#include <QPoint>
#include <QVariant>
#include "mapobject.hpp"
//#include other map objects, as stairs, doors
#include "stair.hpp"
#include "door.hpp"

class GameMap;

/** \brief Single square of GameMap
 */
class MapSquare {
    friend class GameMap;
    public:
        int background;
        //map object
        //container
        ItemsList items;
        Monster * monster;
        ~MapSquare();
    protected:
        bool canAcceptMonster(const Monster& monster);
};

/** \brief Playable map, containing squares, monsters, items, triggers, objects
 */
class GameMap : public QObject{
    Q_OBJECT
    friend class AbstractAction;
    friend class Monster;
    protected:
        int _height;
        int _width;
        QString name;
        int defaultTile;
        MapSquare *squares;
        QSet<Monster*> monsters; //monsters //local to map
        //QStack<Monster*> diedMonsters;
        QList<Trigger*> triggers;
        QMultiMap<QIntsPair,Trigger*> triggersMap;
        //QMultiMap<QIntsPair,QVariantMap> objects; /// map objects
        QMultiMap<QIntsPair, MapObject*> objects;
        // actual objects containers should follow:
        
        
        void clear();
        void initSquares(int defaultTile); // _width and _height must be set
        void insertTrigger(int x, int y, Trigger* trigger);
    public:
        void insertMonster(Monster* monster);
        void removeMonster(Monster* monster);
        GameMap();
        void load(Parser &parser);
        virtual ~GameMap();
        int width();
        int height();
        int backgroundAt(int x, int y);
        MapSquare * squareAt(int x, int y);
        void save(ofstream& file);
        QList<Trigger*> triggersAt(int x, int y);
        QString getName();
        
        // bits of new architecture:
        bool containsMonster(Monster* monster);
        void squareChange(int x, int y);
        void monsterMoved(Monster& monster, int oldX, int oldY);
        bool canAcceptMonsterAt(int x, int y, const Monster& monster);
        
        void generate(const World& world);
        void generateDungeon(QString name, int awidth, int aheight, int& startx, int& starty, int& endx, int& endy, int background, int wall);
        bool createStairs(int stairx, int stairy, bool isUp, QString mapName, int x, int y, int tile = -1);
        const Stair* stairsAt(int x, int y, bool isUp) const;
        bool areObjectsAt(int x, int y) const;
        QList<MapObject*> objectsAt(int x, int y) const;
        //doors
        bool createDoor(int x, int y, bool isClosed, int closedTile, int openTile, bool locked, QString code);
        bool areLockedDoorsAt(int x, int y);
        bool areUnlockedDoorsAt(int x, int y);
        bool areClosedDoorsAt(int x, int y);
        bool areOpenDoorsAt(int x, int y);
        QString getDoorCode(int x, int y);
        bool unlockDoor(int x, int y);
        bool lockDoor(int x, int y);
        bool openDoor(int x, int y);
        bool closeDoor(int x, int y);
    signals:
        void squareChanged(int x, int y);
};

#endif
