#include "game.hpp"
#include "gamemap.hpp"
#include "maps.hpp"
#include "../parser/parser.hpp"
#include "tables.hpp"
#include "monsters.hpp"
#include "worlds.hpp"
#include <fstream>
#include <assert.h>

using namespace Qt;

Game:: Game() {
    engine = 0;
    map = 0;
    loaded = false;
    assert(connect(&player, SIGNAL(healthChange()), this, SLOT(onHealthChange())));
    assert(connect(&player, SIGNAL(mapChanged()), this, SLOT(onMapChanged())));
    assert(connect(&player, SIGNAL(playerMoved()), this, SLOT(onPlayerMoved())));
}

void Game:: clear() {
    Monsters::clear();
    player.clear();
    Maps::clear();
    Worlds::clear();
    map = 0;
    engine->changeMap(0);
    loaded = false;
}

Game:: ~Game() {
    clear();
}

void Game:: setEngine(EngineInterface *_engine) {
    engine = _engine;
}

void Game:: load(QString fileName) {
    clear();
    try {
        Parser parser(fileName);
        while (parser.tryNextToken()) {
            int token = parser.lastToken();
            switch (token) {
                case tktMap: {
                    GameMap* map = new GameMap();
                    if (!map) throw 0;
                    map->load(parser);
                    break;
                }
                case tktPlayer: {
                    player.load(parser);
                    //show player's health
                    engine->repaintStats(player);
                    break;
                }
                case tktWorld: {
                    World* world = new World();
                    if (!world) throw 0;
                    world->load(parser);
                    break;
                }
                case tktGenerate: {
                    int token = parser.nextToken();
                    switch (token) {
                        case tktWorld: {
                            World* world = new World();
                            if (!world) throw 0;
                            world->load(parser, true);
                            GameMap* map = new GameMap();
                            if (!map) throw 0;
                            map->generate(*world);
                            foreach (const Ruin& ruin, world->ruins()) {
                                generateRuin(ruin, *map);
                            }
                            player.setX(world->ruins()[0].x);
                            player.setY(world->ruins()[0].y);
                            player.setMapName(map->getName());
                            break;
                        }
                        case tktPlayer: {
                            int playerX = player.getX();
                            int playerY = player.getY();
                            QString mapName = player.getMapName();
                            player.random(0);
                            parser >> mapName;
                            player.setMapName(mapName);
                            player.setX(playerX);
                            player.setY(playerY);
                            break;
                        }
                        default: {
                            throw QString("game.cpp: load(): generate: Unexpected token \"%1\"").arg(Tables::tokenToName(token));
                        }
                    }
                    break;
                }
                default: {
                    throw QString("game.cpp: load(): Unexpected token \"%1\"").arg(Tables::tokenToName(token));
                }
            }
        }
        player.finishedLoading();
        GameMap* map = Maps::nameToMap(player.getMapName());
        if (!map) {
            throw QString("player is floating in the void");
        }
        map->insertMonster(&player);
        Monsters::add(&player);
        onMapChanged();
        loaded = true;
    }
    catch (...) {
        clear();
        throw;
    }
}

void Game:: save(QString fileName) {
    if (!player.isAlive()) {
        cerr << "cannot save: player is dead!" << endl;
        return;
    }
    ofstream output(fileName.toStdString().c_str(), ios::out);
    if (!output) {
        throw QString("Unable to create file %1").arg(fileName);
    }
    //save player:
    player.save(output);
    //save maps:
    foreach (GameMap* map, Maps::getAllMaps())
        map->save(output);
    foreach (World* world, Worlds::getAllWorlds())
        world->save(output);
    output.close();
}

void Game:: movePlayer(int dx, int dy) {
    int x = player.getX() + dx;
    int y = player.getY() + dy;
    MapSquare* square = map->squareAt(x, y);
    // if player tried to go out of map, simply ignore
    if (!square) return;
    //if (!square->canAcceptMonsterAt(player)) {
    if (!map->canAcceptMonsterAt(x, y, player)) {
        Monster * monster = square->monster;
        if (monster) {
            player.meleeAttack(*monster);
            if (player.energy < 0) {
                moveAllMonsters();
                cout << endl;
            }
        }
        else if (map->areClosedDoorsAt(x, y)) {
            if (map->areUnlockedDoorsAt(x, y)) {
                map->openDoor(x, y); /// very primitive; must be changed
            }
        }
    }
    else {
        player.moveBy(dx, dy);
        // has player any energy left
        if (player.energy < 0) {
            moveAllMonsters();
        }
        // move all others until player regenerates it's energy
    }
}

void Game:: playerUseStairs(bool isUp) {
    const Stair* stair = map->stairsAt(player.getX(), player.getY(), isUp);
    if (!stair) {
        cout << "there are no stairs here" << endl;
        return;
    }
    player.teleport(stair->getMapName(), stair->getTargetX(), stair->getTargetY());
}

void Game:: waitPlayer(int time) {
    player.energy -= time;
    if (player.energy < 0) {
        moveAllMonsters();
    }
}

void Game:: changeActionPoint(int x, int y) {
    actionX = x;
    actionY = y;
}

void Game:: unlockDoor() {
    cout << "unlock door" << endl;
    Monster& monster = player;
    if (monster.squareRangeTo(actionX, actionY) > 2) {
        cout << "target is too far" << endl;
        return;
    }
    if (map->areClosedDoorsAt(actionX, actionY)) {
        if (map->areLockedDoorsAt(actionX, actionY)) {
            QString code = map->getDoorCode(actionX, actionY);
            if (monster.hasKey(code)) {
                assert(map->unlockDoor(actionX, actionY));
            }
            else {
                cout << "You don't have key " << code.toStdString() << endl;
            }
        }
    }
    else {
        cout << "door must be closed" << endl;
    }
}

void Game:: lockDoor() {
    cout << "lock door" << endl;
    Monster& monster = player;
    if (monster.squareRangeTo(actionX, actionY) > 2) {
        cout << "target is too far" << endl;
        return;
    }
    if (map->areClosedDoorsAt(actionX, actionY)) {
        if (map->areUnlockedDoorsAt(actionX, actionY)) {
            QString code = map->getDoorCode(actionX, actionY);
            if (monster.hasKey(code)) {
                assert(map->lockDoor(actionX, actionY));
            }
            else {
                cout << "You don't have key " << code.toStdString() << endl;
            }
        }
    }
    else {
        cout << "door must be closed" << endl;
    }
}

void Game:: openDoor() {
//     cout << "open door" << endl;
    if (player.squareRangeTo(actionX, actionY) > 2) {
        cout << "target is too far" << endl;
        return;
    }
    if (map->areClosedDoorsAt(actionX, actionY)) {
        if (map->areLockedDoorsAt(actionX, actionY)) {
            cout << "door is locked" << endl;
        }
        else {
            map->openDoor(actionX, actionY); /// very primitive; must be changed
            cout << "door opened" << endl;
            player.energyMove();
        }
    }
}

void Game:: closeDoor() {
//     cout << "close door" << endl;
    if (player.squareRangeTo(actionX, actionY) > 2) {
        cout << "target is too far" << endl;
        return;
    }
    if (map->areOpenDoorsAt(actionX, actionY)) {
        map->closeDoor(actionX, actionY); /// very primitive; must be changed
    }
}

void Game:: generateRuin(const Ruin& ruin, GameMap& mainMap) {
    switch (ruin.type) {
        case Dungeon: {
            GameMap* map = new GameMap();
            if (!map) throw 0;
            int startx, starty, endx, endy;
            BackgroundClass criteria;
            criteria.flags = bkgFlgUnderground;
            int background = Tables::search(criteria);
            assert(background >= 0); /// \todo replace with error
            criteria.flags = bkgFlgObstacle | bkgFlgBulletproof | bkgFlgVisionproof | bkgFlgUnderground;
            int wall = Tables::search(criteria);
            assert(wall >= 0); /// \todo replace with error
            map->generateDungeon(ruin.name, 40, 40, startx, starty, endx, endy, background, wall);
            mainMap.createStairs(ruin.x, ruin.y, false, map->getName(), startx, starty);
            QVariantMap oCriteria;
            oCriteria["type"] = "stair";
            oCriteria["isup"] = true;
            int tile = Tables::searchObject(oCriteria);
            //cout << "tile=" << tile << endl;
            map->createStairs(startx, starty, true, mainMap.getName(), ruin.x, ruin.y, tile);
            break;
        }
        default: {
            assert(false);
        }
    }
}

void Game:: moveAllMonsters() {
    QSet<Monster*>& monsters = Monsters::getAllMonsters();
    do {
        foreach (Monster* monster, monsters) {
            if (!monster->isAlive()) continue;
            monster->naturallyRegenerate();
            if (!monster->isMonster()) {
                continue;
            }
            while (monster->energy >= 0) {
                monster->aiMove();
                if (!player.isAlive()) return;
            }
        }
    } while (player.energy < 0);
    removeDeadMonsters();
}

void Game:: removeDeadMonsters() {
    QSet<Monster*>& allMonsters = Monsters::getAllMonsters();
    foreach (Monster* monster, allMonsters) {
        if (!monster->isAlive()) {
            if (!monster->isMonster()) {
                cout << "Game::removeDeadMonsters(): player died" << endl;
                continue;
            }
            ItemsList items = monster->takeAllItems();
            GameMap* map = Maps::nameToMap(monster->getMapName());
            int x = monster->getX();
            int y = monster->getY();
            MapSquare* square = map->squareAt(x, y);
            square->items += items;
            map->removeMonster(monster);
            map->squareChange(x, y);
            delete monster;
        }
    }
}

void Game:: onKeyPress(QKeyEvent * event) {
    if (!player.isAlive()) {
        if (loaded) {
            cout << "You are dead. load or quit." << endl;
        }
        return;
    }
    if (!loaded) return;
    switch (event->key()) {
        case Key_Left: {
            movePlayer(-1, 0);
            break;
        }
        case Key_Right: {
            movePlayer(1, 0);
            break;
        }
        case Key_Up: {
            movePlayer(0, -1);
            break;
        }
        case Key_Down: {
            movePlayer(0, 1);
            break;
        }
        case Key_Comma: {
            ItemsList& groundItems = map->squareAt(player.getX(), player.getY())->items;
            if (groundItems.count() == 0) {
                cout << "Nothing to pick up" << endl;
            }
            else if (groundItems.count() == 1) {
                player.items.append(groundItems.takeAllAt(0));
                engine->updateSquare(player.getX(), player.getY());
                waitPlayer();
                cout << "One item picked up" << endl;
            }
            else { //many items on ground
                cout << "Several items on ground" << endl;
                QBitArray selection = engine->getPickupSelection(groundItems);
                cout << "Picking " << selection.count(true) << " items" << endl;
                for (int i=selection.size()-1; i>=0; i--) {
                    if (selection[i]) {
                        // TODO: option to take not all stack items
                        cout << "item picked up" << endl;
                        player.items.append(groundItems.takeAllAt(i));
                    }
                }
                engine->updateSquare(player.getX(), player.getY());
                waitPlayer();
            }
            break;
        }
        case Key_Period: {
            waitPlayer();
            cout << "You wait" << endl;
            break;
        }
        case Key_Colon: {
            cout << "Colon" << endl;
            break;
        }
        case Key_Semicolon: {
            cout << "Semicolon" << endl;
            break;
        }
        case Key_Less: {
            playerUseStairs(true);
            break;
        }
        case Key_Greater: {
            playerUseStairs(false);
            break;
        }
        case Key_I: {
            cout << "Should show inventory" << endl;
            engine->manageEquipment(&player);
            break;
        }
        default: {
            //cout << event->text().toStdString() << endl;
        }
    }
}

void Game:: onHealthChange() {
    QStatsEvent event(player.getHp(), player.getMaxHp(), player.getFp(), player.getMaxFp(), player.getMp(), player.getMaxMp());
    emit repaintStats(&event);
}

void Game:: onUpdateSquare(int x, int y) {
    engine->updateSquare(x, y);
}

void Game:: onMapChanged() {
    if (map) {
        assert(disconnect(map, SIGNAL(squareChanged(int,int)), this, SLOT(onUpdateSquare(int,int))));        map = 0;
    }
    map = Maps::nameToMap(player.getMapName());
    assert(map != 0);
    assert(connect(map, SIGNAL(squareChanged(int,int)), this, SLOT(onUpdateSquare(int,int))));
    engine->changeMap(map);
    engine->focus(player.getX(), player.getY());
}

void Game:: onSquareDoubleClicked(int x, int y) {
    cout << QString("double clicked at %1, %2").arg(x).arg(y).toStdString() << endl;
    if (!map) return;
    if (!map->canAcceptMonsterAt(x, y, player)) {
        cout << "can accept" << endl;
    }
    else {
        cout << "cannot accept" << endl;
    }
}

void Game:: onPlayerMoved() {
    engine->focus(player.getX(), player.getY());
}
