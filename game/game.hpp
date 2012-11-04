#ifndef GAME_HPP_
#define GAME_HPP_

#include "engineinterface.hpp"
#include "gamemap.hpp"
#include "monster.hpp"
#include "player.hpp"
#include "monsterclass.hpp"
#include "masks.hpp"
#include "../engine/qstatsevent.hpp"
#include "world.hpp"
#include <QObject>
#include <QKeyEvent>
#include <iostream>

using namespace std;

class Game : public QObject {
    Q_OBJECT
    protected:
        EngineInterface *engine;
        bool loaded;
        Player player; // main player character
        GameMap* map;
        //World world;
        
        int actionX;
        int actionY;
        
        void clear();
        void moveAllMonsters();
        void removeDeadMonsters();
        //generating
        void generateRuin(const Ruin& ruin, GameMap& mainMap);
    public:
        Game();
        virtual ~Game();
        void setEngine(EngineInterface *_engine);
        void load(QString fileName);
        void save(QString fileName);
        void movePlayer(int dx, int dy);
        void playerUseStairs(bool isUp);
        void waitPlayer(int time = 100);
    public slots:
        void onKeyPress(QKeyEvent * event);
        void onHealthChange();
        void onUpdateSquare(int x, int y);
        void onMapChanged();
        void onSquareDoubleClicked(int x, int y);
        void onPlayerMoved();
        // menu actions
        void changeActionPoint(int x, int y);
        void unlockDoor();
        void lockDoor();
        void openDoor();
        void closeDoor();
    signals:
        void repaintStats(QStatsEvent * event);
};

#endif
