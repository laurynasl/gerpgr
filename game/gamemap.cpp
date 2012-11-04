#include "gamemap.hpp"
#include "masks.hpp"
#include "tables.hpp"
#include "triggerquestion.hpp"
#include "triggerplayer.hpp"
#include "actionrandom.hpp"
#include "actiontravel.hpp"
#include "monsters.hpp"
#include "maps.hpp"
#include <assert.h>
#include <math.h>
#include "../algorithms/graph.hpp"
#include "../iterators/lineiterator.hpp"
#include "../iterators/walliterator.hpp"

GameMap:: GameMap() :
        _height(0), _width(0), defaultTile(0), squares(0) {
}

void GameMap:: clear() {
    foreach (Trigger* trigger, triggers) {
        delete trigger;
    }
    triggers.clear();
    triggersMap.clear();
    foreach (Monster* monster, monsters) {
        if (monster->isMonster()) {
            delete monster;
        }
    }
    monsters.clear();
    foreach (MapObject* object, objects) {
        delete object;
    }
    objects.clear();
    if (squares) {
        delete[] squares;
        squares = 0;
    }
}

void GameMap:: initSquares(int defaultTile) {
    clear();
    int size = _width * _height;
    squares = new MapSquare[size];
    for (int i=0; i< size; i++) {
        squares[i].background = defaultTile;
        squares[i].monster = 0;
    }
}

GameMap:: ~GameMap() {
    clear();
}

int GameMap:: width() {
    return _width;
}

int GameMap:: height() {
    return _height;
}

int GameMap:: backgroundAt(int x, int y) {
    return squares[x + y * _width].background;
}

void GameMap:: insertMonster(Monster* monster) {
    monsters.insert(monster);
    MapSquare * square = squareAt(monster->getX(), monster->getY());
    square->monster = monster;
    emit squareChanged(monster->getX(), monster->getY());
}

void GameMap:: removeMonster(Monster* monster) {
    monsters.remove(monster);
    if (squares) {
        squareAt(monster->getX(), monster->getY())->monster = 0;
    }
}

void GameMap:: load(Parser &parser) {
    QString backgroundName;
    Maps::remove(this);
    parser >> name >> _width >> _height >> backgroundName;
    Maps::add(this); // name is loaded, can add
    
    defaultTile = Tables::nameToBackground(backgroundName);
    initSquares(defaultTile);
    int token;
    try {
        do {
            token = (TokenType) parser.nextToken();
            switch (token) {
                case tktTile:{
                    QString name;
                    int x, y;
                    parser >> name >> x >> y;
                    squares[x + y * _width].background = Tables::nameToBackground(name);
                    break;
                }
                case tktTilesRange:{
                    QString name;
                    int x, y, w, h;
                    parser >> name >> x >> y >> w >> h;
                    int tile = Tables::nameToBackground(name);
                    for (int i = x; i<x+w; i++) {
                        for (int j=y; j<y+h; j++) {
                            squares[i + j * _width].background = tile;
                        }
                    }
                    break;
                }
                case tktMonster: {
                    Monster * monster = new Monster();
                    monster->setMapName(name);
                    monster->load(parser);
                    monster->name = Tables::monsterToClass(monster->getMonsterType()).name;
                    monster->finishedLoading();
                    insertMonster(monster);
                    Monsters::add(monster);
                    break;
                }
                /*case tktRandom: {
                    token = (TokenType)parser.nextToken();
                    if ((token == tktMonster) || (token == tktMonsters)) {
                        int monstersCount = 1;
                        if (token == tktMonsters) {
                            parser >> monstersCount;
                        }
                        QString className;
                        parser >> className;
                        int monsterType = Tables::nameToMonster(className);
                        token = (TokenType) parser.nextToken();
                        int x, y, w, h;
                        parser >> x >> y;
                        if (token == tktAt) {
                            w = 1;
                            h = 1;
                        }
                        else if (token == tktIn) {
                            parser >> w >> h;
                        }
                        else {
                            throw EException("UTA: \"random monster(s)\"");
                        }
                        //now we have monsters' type and count and all coordinates
                        for (int i=0; i<monstersCount; i++) {
                            Monster * monster = new Monster();
                            monster->setMapName(name);
                            monster->random(monsterType);
                            //try to place monster up to 50 times
                            bool inserted = false;
                            for (int j=0; j<50; j++) {
                                int xpos = x + rand() % w;
                                int ypos = y + rand() % h;
                                monster->x = xpos;
                                monster->y = ypos;
                                MapSquare * square = squareAt(xpos, ypos);
                                if (square->canAcceptMonster(*monster)) {
                                    insertMonster(monster);
                                    inserted = true;
                                    break;
                                }
                            }
                            if (!inserted) {
                                delete monster;
                            }
                        }
                        cout << "random OK" << endl;
                    }
                    else {
                        throw EException("gamemap.cpp: UTA: \"random\"");
                    }
                    break;
                }*/
                case tktTrigger: {
                    int t = parser.nextToken();//trigger name
                    Trigger* trigger;
                    switch(t) {
                        case tktPlayer: {
                            trigger = new TriggerPlayer();
                            break;
                        }
                        case tktQuestion: {
                            trigger = new TriggerQuestion();
                            break;
                        }
                        default: {
                            throw EException("GameMap::load().tktTrigger2: Unexpected token");
                        }
                    }
                    try {
                        trigger->load(parser);
                    }
                    catch (...) {
                        delete trigger;
                        throw;
                    }
                    triggers.append(trigger);
                    QList<QIntsPair> points = trigger->getPoints();
                    foreach (const QIntsPair& point, points){
                        triggersMap.insert(point, trigger);
                    }
                    token = -1;
                    break;
                }
                case tktItem: {
                    int x, y, count;
                    QString itemName;
                    parser >> count >> itemName >> x >> y;
                    Item item(Tables::nameToItem(itemName), count);
                    MapSquare* square = squareAt(x, y);
                    if (square) {
                        square->items.append(item);
                    }
                    else {
                        throw EException(QString("GameMap::load(): tktItem: no such coordinates %1 %2").arg(x).arg(y));
                    }
                    break;
                }
                case tktEnd: break;
                default:
                    throw EException("GameMap::load(): Unexpected token");
            }
        } while (token != tktEnd);
    }
    catch (QString& e) {
        cout << "[E] GameMap::load(): " << e.toStdString() << endl;
        clear();
        throw;
    }
    catch (EException& e) {
        cout << "[E] GameMap::load(): " << e.message().toStdString() << endl;
        clear();
        throw;
    }
    catch (...) {
        cout << "[E] GameMap::load()" << endl;
        clear();
        throw;
    }
}

MapSquare * GameMap:: squareAt(int x, int y) {

    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
        return 0;
    }
    return &squares[x + y * _width];
}

void GameMap:: save(ofstream& file) {
    QString defaultTileName = Tables::backgroundToClass(defaultTile).name;
    file << "map \"" << name.toStdString() << "\" ";
    file << _width << " " << _height << " ";
    file << "\"" << defaultTileName.toStdString() << "\"" << endl;
    // save squares
    // lame implementation:
    for (int j=0; j<_height; j++) {
        for (int i=0; i<_width; i++) {
            MapSquare* square = squareAt(i, j);
            if (square->background != defaultTile) {
                file << "    tile \"" << Tables::backgroundToClass(square->background).name.toStdString();
                file << "\" " << i << " " << j << endl;
            }
            // save items
            if (square->items.count()) {
                for (int k=0; k<square->items.count(); k++) {
                    file << "    item " << square->items[k].count() << " \"";
                    file << square->items[k].name().toStdString();
                    file << "\" " << i << " " << j << endl;
                }
            }
        }
    }
    // save monsters
    foreach (Monster* monster, monsters) {
        if (monster->isMonster()) {
            monster->save(file);
        }
    }
    // save triggers
    foreach (Trigger* trigger, triggers)
        trigger->save(file);
    file << "end" << endl << endl;
}

QList<Trigger*> GameMap:: triggersAt(int x, int y) {
    return triggersMap.values(QIntsPair(x,y));
}

QString GameMap:: getName() {
    return name;
}

bool GameMap:: containsMonster(Monster* monster) {
    return monsters.contains(monster);
}

void GameMap:: squareChange(int x, int y) {
    emit squareChanged(x, y);
}

void GameMap:: monsterMoved(Monster& monster, int oldX, int oldY) {
    squareAt(oldX, oldY)->monster = 0;
    squareAt(monster.getX(), monster.getY())->monster = &monster;
    emit squareChanged(oldX, oldY);
    emit squareChanged(monster.getX(), monster.getY());
    
    QList<Trigger*> t = triggersAt(monster.getX(), monster.getY());
    foreach (Trigger* trigger, t) {
        if (!trigger->activate(monster)) {
            break;
        }
    }
}

bool GameMap:: canAcceptMonsterAt(int x, int y, const Monster& monster) {
    MapSquare* square = squareAt(x, y);
    if (!square) return false;
    if (!square->canAcceptMonster(monster)) return false;
    if (areObjectsAt(x, y)) {
        const QList<MapObject*> objects = objectsAt(x, y);
        foreach (const MapObject* object, objects) {
            if (object->isObstacle()) return false;
        }
    }
    return true;
}

void GameMap:: generate(const World& world) {
    Maps::remove(this);
    clear();
    name = world.name();
    Maps::add(this);
    _width = world.width();
    _height = world.height();
    initSquares(0);
    for (int i=0; i<_width; i++) {
        for (int j=0; j<_height; j++) {
            float z = world.heightMap()[i][j];
            MapSquare* square = squareAt(i, j);
            assert(square != 0);
            BackgroundClass bClass;
            bClass.flags = bkgFlgForMainmap;
            if (z < 0) bClass.flags |= bkgFlgObstacle;
            bClass.height = (int)z;
            int background = Tables::search(bClass);
            assert(background >= 0); /// \todo replace with error
            square->background = background;
        }
    }
    foreach (const Ruin& ruin, world.ruins()) {
        BackgroundClass criteria;
        criteria.height = (int)world.heightMap()[ruin.x][ruin.y];
        criteria.flags = bkgFlgForMainmap;
        criteria.type = "ruin";
        int background = Tables::search(criteria);
        assert(background >= 0);
        MapSquare* square = squareAt(ruin.x, ruin.y);
        assert(square != 0);
        square->background = background;
    }
}

void GameMap:: generateDungeon(QString name, int awidth, int aheight, int& startx, int& starty, int& endx, int& endy, int background, int wall) {
    assert(awidth >= 25);
    assert(aheight >= 25);
    Maps::remove(this);
    clear();
    this->name = name;
    Maps::add(this);
    _width = awidth;
    _height = aheight;
    initSquares(-1); //invalid background. will be used for room placing
    /// room sizes are including walls, so in fact they are smaller by 2
    int roomMinW = 6;
    int roomMaxW = 10;
    int roomMinH = 5;
    int roomMaxH = 8;
    /// lets say that rooms will occupy 25% of whole space
    int roomsCount = awidth * aheight * 4 / (roomMinW + roomMaxW) / (roomMinH + roomMaxH) / 4/*==25%*/;
    assert(roomsCount > 1);
    QList<Coordinate> roomsList;
    for (int index=0; index<roomsCount; index++) {
        int x, y, w, h;
        bool found = false;
        for (int index2=0; index2<50; index2++) {
            w = roomMinW + rand() % (roomMaxW - roomMinW + 1);
            h = roomMinH + rand() % (roomMaxH - roomMinH + 1);
            x = rand() % (_width - w);
            y = rand() % (_height - h);
            // test whether room can be placed at these coordinates
            found = true;
            try {
                for (int i=0; i<w; i++) {
                    for (int j=0; j<h; j++) {
                        MapSquare* square = squareAt(x+i, y+j);
                        assert(square != 0);
                        if (square->background != -1) {
                            throw 0;
                        }
                    }
                }
            }
            catch (int value) {
                found = false;
            }
            if (found) {
                break;
            }
        }
        Coordinate roomCoordinate(x, y, w, h);
        roomsList.append(roomCoordinate);
        // now that I have coordinates and size of a room, I place it
        for (int i=0; i<w; i++) {
            for (int j=0; j<h; j++) {
                MapSquare* square = squareAt(x+i, y+j);
                assert(square != 0);
                if ((i == 0) || (i == (w-1)) || (j == 0) || (j == (h-1))) {
                    square->background = wall;
                }
                else {
                    square->background = background;
                }
            }
        }
    }
    // start position is in the middle of first room
    startx = roomsList[0].w / 2 + roomsList[0].x;
    starty = roomsList[0].h / 2 + roomsList[0].y;
    // end position is in the middle of second room
    endx = roomsList[1].w / 2 + roomsList[1].x;
    endy = roomsList[1].h / 2 + roomsList[1].y;
    //now fill all invalid space as walls
    for (int i=0; i<_width; i++) {
        for (int j=0; j<_height; j++) {
            MapSquare* square = squareAt(i, j);
            assert(square != 0);
            if (square->background == -1) {
                square->background = wall;
            }
        }
    }
    Graph fullGraph;
    fullGraph.createDisconnectedGraph(roomsCount);
    for (int i=1; i<roomsCount; i++) {
        for (int j=0; j<i; j++) {
            int dx = roomsList[i].x - roomsList[j].x;
            int dy = roomsList[i].y - roomsList[j].y;
            int weight = (int) sqrt((double)dx * dx + dy * dy);
            fullGraph.connect(i, j, weight);
        }
    }
    Graph graph = fullGraph.getMST();
    // now connect rooms with tunnels
    for (int i=1; i<roomsCount; i++) {
        for (int j=0; j<i; j++) {
            if (graph.edgeWeight(i, j) == -1) {
                continue;
            }
            //cout << QString("rooms %1 and %2 are connected").arg(i).arg(j).toStdString() << endl;
            // so, connect two rooms from center to center...
            int x1 = roomsList[i].x + roomsList[i].w / 2;
            int y1 = roomsList[i].y + roomsList[i].h / 2;
            int x2 = roomsList[j].x + roomsList[j].w / 2;
            int y2 = roomsList[j].y + roomsList[j].h / 2;
            //cout << x1 << "," << y1 << " ; " << x2 << "," << y2 << endl;
            for (LineIterator line(x1, y1, x2, y2); !line; line++) {
                //cout << line.x() << " " << line.y() << endl;
                MapSquare* square = squareAt(line.x(), line.y());
                assert(square != 0);
                square->background = background;
            }
        }
    }
    // now create doors
    for (int i=0; i<roomsCount; i++) {
        const Coordinate& room = roomsList[i];
        for (WallIterator wall(room.x, room.y, room.w, room.h); !wall; wall++) {
            MapSquare* square = squareAt(wall.x(), wall.y());
            assert(square != 0);
            if (square->background == background) {
                QVariantMap doorCriteria;
                doorCriteria["type"] = "door";
                doorCriteria["closed"] = true;
                int closedTile = Tables::searchObject(doorCriteria);
                doorCriteria["closed"] = false;
                int openTile = Tables::searchObject(doorCriteria);
                // then create door here...
                bool isClosed = (rand() % 100) >= 50;
                //Door* door = new Door(
                assert(createDoor(wall.x(), wall.y(), isClosed, closedTile, openTile, false, Tables::getRandomKeyCode()));
            }
        }
    }
}

bool GameMap:: createStairs(int stairx, int stairy, bool isUp, QString mapName, int x, int y, int tile) {
    if (stairsAt(stairx, stairy, isUp)) {
        return false;
    }
    Stair* stair = new Stair(mapName, x, y, isUp, tile);
    assert(stair); //crash fast, if out of memory
    QIntsPair pair(stairx, stairy);
    objects.insert(pair, stair);
    return true;
}

const Stair* GameMap:: stairsAt(int x, int y, bool isUp) const {
    QIntsPair pair(x, y);
    if (!objects.contains(pair)) return 0;
    foreach (const MapObject* object, objects.values(pair)) {
        if (object->getClassName() == "stair") {
            const Stair* stair = (const Stair*) object;
            if (stair->isUp() == isUp) {
                return stair;
            }
        }
    }
    return 0;
}

bool GameMap:: areObjectsAt(int x, int y) const {
    QIntsPair pair(x, y);
    return objects.contains(pair);
}

QList<MapObject*> GameMap:: objectsAt(int x, int y) const {
    QIntsPair pair(x, y);
    return objects.values(pair);
}

bool GameMap:: createDoor(int x, int y, bool isClosed, int closedTile, int openTile, bool locked, QString code) {
    Door* door = new Door(isClosed, closedTile, openTile, locked, code);
    assert(door); //crash fast, if out of memory
    QIntsPair pair(x, y);
    objects.insert(pair, door);
    return true;
}

bool GameMap:: areLockedDoorsAt(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (door->isLocked()) return true;
    }
    return false;
}

bool GameMap:: areUnlockedDoorsAt(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (!door->isLocked()) return true;
    }
    return false;
}

bool GameMap:: areClosedDoorsAt(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (door->isClosed()) return true;
    }
    return false;
}

bool GameMap:: areOpenDoorsAt(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (!door->isClosed()) return true;
    }
    return false;
}

QString GameMap:: getDoorCode(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        return door->getCode();
    }
    return "";
}

bool GameMap:: unlockDoor(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (door->isLocked()) {
            assert(door->unlock());
//             emit squareChanged(x, y);
            return true;
        }
    }
    return false;
}

bool GameMap:: lockDoor(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (!door->isLocked()) {
            assert(door->lock());
//             emit squareChanged(x, y);
            return true;
        }
    }
    return false;
}

bool GameMap:: openDoor(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (door->isClosed()) {
            assert(door->open());
            emit squareChanged(x, y);
            return true;
        }
    }
    return false;
}

bool GameMap:: closeDoor(int x, int y) {
    if (!areObjectsAt(x, y)) return false;
    QList<MapObject*> objects = objectsAt(x, y);
    foreach (MapObject* object, objects) {
        if (object->getClassName() != "door") continue;
        Door* door = (Door*) object;
        if (!door->isClosed()) {
            assert(door->close());
            emit squareChanged(x, y);
            return true;
        }
    }
    return false;
}

// ********************** MapSquare *********************
bool MapSquare:: canAcceptMonster(const Monster & monster) {
    assert(&monster != 0);
    if (this->monster) return false;
    /// \todo other tests, like obstacles, monster skills
    if (Tables::backgroundToClass(background).isObstacle()) {
        return false;
    }
    return true;
}

MapSquare:: ~MapSquare() {
}
