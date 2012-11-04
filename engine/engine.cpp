#include "engine.hpp"
#include <QPainter>
#include <QColor>
#include <QMessageBox>
#include <QMenu>
#include "../game/gamemap.hpp"
#include "../parser/parser.hpp"
#include "../game/exception.hpp"
#include "../game/tables.hpp"
#include <fstream>
#include <iostream>
#include <assert.h>

#define DEFAULT_SQUARE_X 32;
#define DEFAULT_SQUARE_Y 32;

using namespace std;
using namespace Qt;

Engine:: Engine() {
    map = 0;
    squareX = DEFAULT_SQUARE_X;
    squareY = DEFAULT_SQUARE_Y;
    cameraX = 0;
    cameraY = 0;
    cameraWidth = width() / squareX;
    cameraHeight = height() / squareY;
    //tiles0 -- backgroundTiles
    {
        Parser parser("data/tiles0");
        backgroundTiles.resize(Tables::backgroundsCount());
        QString name = parser.getString();
        QString fname;
        do {
            parser >> fname;
            int background = Tables::nameToBackground(name);
            QDiscardablePixmap* pixmap = filenameToPixmap(fname);
            if (backgroundTiles[background]) {
                cerr << "engine.cpp: Warning: data/tiles0 overwrite of pixmap" << endl;
            }
            backgroundTiles[background] = pixmap;
        } while (!(name = parser.getString()).isEmpty());
    }
    //tiles0 -- objectTiles
    {
        Parser parser("data/tiles1");
        objectTiles.resize(Tables::objectsCount());
        QString name = parser.getString();
        QString fname;
        do {
            parser >> fname;
            int object = Tables::nameToObject(name);
            QDiscardablePixmap* pixmap = filenameToPixmap(fname);
            if (objectTiles[object]) {
                cerr << "engine.cpp: Warning: data/tiles1 overwrite of pixmap" << endl;
            }
            objectTiles[object] = pixmap;
        } while (!(name = parser.getString()).isEmpty());
    }
    {
        Parser parser("data/tiles3");
        itemTiles.resize(Tables::itemsCount());
        QString name = parser.getString();
        QString fname;
        do {
            parser >> fname;
            int item = Tables::nameToItem(name);
            QDiscardablePixmap* pixmap = filenameToPixmap(fname);
            if (itemTiles[item]) {
                cerr << "engine.cpp: Warning: data/tiles3 overwrite of pixmap" << endl;
            }
            itemTiles[item] = pixmap;
        } while (!(name = parser.getString()).isEmpty());
    }
    {
        Parser parser("data/tiles4");
        monsterTiles.resize(Tables::monstersCount());
        QString name = parser.getString();
        QString fname;
        do {
            parser >> fname;
            int monster = Tables::nameToMonster(name);
            QDiscardablePixmap* pixmap = filenameToPixmap(fname);
            if (monsterTiles[monster]) {
                cerr << "engine.cpp: Warning: data/tiles4 overwrite of pixmap" << endl;
            }
            monsterTiles[monster] = pixmap;
        } while (!(name = parser.getString()).isEmpty());
    }
    
    // construct actions
    unlockDoorAction = new QAction("Unlock door", this);
//     assert(connect(unlockDoorAction, SIGNAL(triggered()), this, SLOT(unlockDoor())));
    lockDoorAction = new QAction("Lock door", this);
//     assert(connect(lockDoorAction, SIGNAL(triggered()), this, SLOT(lockDoor())));
    openDoorAction = new QAction("Open door", this);
//     assert(connect(openDoorAction, SIGNAL(triggered()), this, SLOT(openDoor())));
    closeDoorAction = new QAction("Close door", this);
//     assert(connect(closeDoorAction, SIGNAL(triggered()), this, SLOT(closeDoor())));
    
}

Engine:: ~Engine() {
    foreach (QDiscardablePixmap* pixmap, pixmaps)
        delete pixmap;
}

void Engine:: changeMap(GameMap *map) {
    this->map = map;
    update();
}

void Engine:: paintEvent(QPaintEvent * event) {
    if (!map) return;
    QPainter painter(this);
    
    const QRect &rect = event->rect();
    int x1 = rect.x() / squareX;
    int y1 = rect.y() / squareY;
    int x2 = (rect.x() + rect.width()) / squareX + 1;
    int y2 = (rect.y() + rect.height()) / squareY + 1;
    
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            MapSquare *square = map->squareAt(x+cameraX, y+cameraY);
            if (!square) continue;
            QPixmap &pixmap = backgroundTiles[square->background]->pixmap();
            painter.drawPixmap(squareX * x, squareY * y, squareX, squareY, pixmap);
            if (map->areObjectsAt(x+cameraX, y+cameraY)) {
                QList<MapObject*> list = map->objectsAt(x+cameraX, y+cameraY);
                foreach(const MapObject* object, list) {
                    if (object->getTile() >= 0) {
                        QPixmap& pixmap = objectTiles[object->getTile()]->pixmap();
                        painter.drawPixmap(squareX * x, squareY * y, squareX, squareY, pixmap);
                    }
                }
            }
            if (!square->items.empty()) {
                int itemType = square->items.last().type();
                QPixmap& pixmap = itemTiles[itemType]->pixmap();
                painter.drawPixmap(squareX * x, squareY * y, squareX, squareY, pixmap);
            }
            
            if (square->monster) {
                QPixmap &pixmap = monsterTiles[
                        square->monster->getMonsterType()]->pixmap();
                painter.drawPixmap(squareX * x, squareY * y, squareX, squareY, pixmap);
//                 painter.setPen(QColor(255, 255, 255));
//                 painter.drawText(squareX * x, squareY * (y+1), QString("%1/%2").arg(square->monster->getHp()).arg(square->monster->getFp()));
            }
        }
    }
    painter.end();
}

void Engine:: monsterMoved(Monster &monster, int dx, int dy) {
    //just temporary simple "jump" to another square
    //TODO make it move using frames
    monsterMovedTo(monster, monster.getX() - dx, monster.getY() - dy);
}

void Engine:: monsterMovedTo(Monster &monster, int oldX, int oldY) {
    updateSquare(oldX, oldY);
    updateSquare(monster.getX(), monster.getY());
}

void Engine:: keyPressEvent(QKeyEvent * event) {
    emit onKeyPress(event);
}

void Engine:: mouseDoubleClickEvent(QMouseEvent* event) {
    switch (event->button()) {
        case LeftButton: {
            emit onSquareDoubleClicked(event->x() / squareX, event->y() / squareY);
            break;
        }
        default: {
        }
    }
}

void Engine:: mousePressEvent(QMouseEvent* event) {
    int x = event->x() / squareX + cameraX;
    int y = event->y() / squareY + cameraY;
    switch (event->button()) {
        case RightButton: {
            if (!map) {
                break;
            }
            // emit current coordinates somewhere in engine
            emit onActionPointChanged(x, y);
            
            // select actions
            QList<QAction*> actions;
            QList<MapObject*> objects = map->objectsAt(x, y);
            foreach (MapObject* object, objects) {
                if (object->getClassName() == "door") {
                    Door* door = (Door*) object;
                    if (door->isClosed()) {
                        actions.append(openDoorAction);
                        if (door->isLocked()) {
                            actions.append(unlockDoorAction);
                        }
                        else {
                            actions.append(lockDoorAction);
                        }
                    }
                    else {
                        actions.append(closeDoorAction);
                    }
                }
            }
            // if selection is not empty, pop up menu
            if (!actions.empty()) {
                QMenu::exec(actions, mapToGlobal(event->pos()));
            }
            break;
        }
        case MidButton: {
            focus(x, y);
            break;
        }
        default: {
        }
    }
}

void Engine:: resizeEvent(QResizeEvent* event) {
    resized();
}

void Engine:: resized() {
    cameraWidth = width() / squareX;
    cameraHeight = height() / squareY;
}

QDiscardablePixmap* Engine:: filenameToPixmap(const QString filename) {
    if (!pixmaps.contains(filename)) {
        pixmaps[filename] = new QDiscardablePixmap(filename, squareX, squareY);
    }
    return pixmaps[filename];
}

void Engine:: repaintStats(Monster &monster) {
    QStatsEvent event(monster.getHp(), monster.getMaxHp(),
                      monster.getFp(), monster.getMaxFp(),
                      monster.getMp(), monster.getMaxMp());
    emit onRepaintStats(&event);
}

void Engine:: updateSquare(int x, int y) {
    x -= cameraX;
    y -= cameraY;
    update(QRect(x * squareX, y * squareY, squareX, squareY));
}

QBitArray Engine:: getPickupSelection(const ItemsList items) {
    return pickupDialog.exec1(items);
}

void Engine:: manageEquipment(Monster* monster) {
    equipmentDialog.exec1(monster);
}

void Engine:: setSquareSize(int size) {
    squareX = (squareY = size);
    foreach (QDiscardablePixmap* pixmap, pixmaps) {
        pixmap->resize(squareX, squareY);
    }
    resized();
    update();
}

int Engine:: getSquareSize() const {
    return squareX; /// \todo but what about squareY?
}

bool Engine:: askPlayerYesNo(const QString& caption, const QString& text) {
    return QMessageBox::question(this, caption, text, QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes;
}

void Engine:: focus(int x, int y) {
    // if required coordinates are not inside a half of camera's rectangle,
    // coordinates will be in the middle of camera's rectangle.
    // but for each coordinate separately
    if (!map) {
        cerr << "unable to focus: no map loaded" << endl;
        return;
    }
    int newCameraX = cameraX;
    int newCameraY = cameraY;
    if ((x < cameraX + cameraWidth / 4) || (x > cameraX + cameraWidth * 3 / 4)) {
        newCameraX = max(x - cameraWidth / 2, 0);
        newCameraX = min(newCameraX, max(0, map->width() - cameraWidth));
    }
    if ((y < cameraY + cameraHeight / 4) || (y > cameraY + cameraHeight* 3 / 4)) {
        newCameraY = max(y - cameraHeight / 2, 0);
        newCameraY = min(newCameraY, max(0, map->height() - cameraHeight));
    }
    if ((newCameraX != cameraX) || (newCameraY != cameraY)) {
        cameraX = newCameraX;
        cameraY = newCameraY;
        update();
    }
}
