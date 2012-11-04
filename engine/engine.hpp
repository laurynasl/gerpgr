#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <QWidget>
#include <QList>
#include <QVector>
#include <QKeyEvent>
#include <QAction>
#include "../game/engineinterface.hpp"
#include "../game/gamemap.hpp"
#include "../game/monster.hpp"
#include "qstatsevent.hpp"
#include "qdiscardablepixmap.hpp"
#include "QPickupDialog.hpp"
#include "QEquipmentDialog.hpp"

/** \brief Default 2D engine with widgets, keyboard, mouse
 */
class Engine : public QWidget, public EngineInterface {
    Q_OBJECT
    protected:
        GameMap *map;
        int squareX; /// width of single square, in points
        int squareY; /// height of single square, in points
        
        int cameraX; /// leftmost square still visible in camera
        int cameraY; /// topmost square still visible in camera
        int cameraWidth; /// camera width, in squares
        int cameraHeight; /// camera height, in squares
        QVector<QDiscardablePixmap*> backgroundTiles;
        QVector<QDiscardablePixmap*> itemTiles;
        QVector<QDiscardablePixmap*> monsterTiles;
        QVector<QDiscardablePixmap*> objectTiles;
        // new universal system for pixmaps
        QMap<QString,QDiscardablePixmap*> pixmaps;
        QPickupDialog pickupDialog;
        QEquipmentDialog equipmentDialog;
        
        virtual void paintEvent(QPaintEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void mouseDoubleClickEvent(QMouseEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void resizeEvent(QResizeEvent* event);
        virtual void resized();
        QDiscardablePixmap* filenameToPixmap(const QString filename);
    public:
        QAction* unlockDoorAction;
        QAction* lockDoorAction;
        QAction* openDoorAction;
        QAction* closeDoorAction;
        
        Engine();
        virtual ~Engine();
        virtual void changeMap(GameMap* map);
        virtual void monsterMoved(Monster &monster, int dx, int dy);
        virtual void monsterMovedTo(Monster &monster, int oldX, int oldY);
        // basically health, TODO: attributes
        virtual void repaintStats(Monster &monster);
        virtual void updateSquare(int x, int y);
        virtual QBitArray getPickupSelection(const ItemsList items);
        virtual void manageEquipment(Monster* monster);
        virtual void setSquareSize(int size);
        virtual int getSquareSize() const;
        virtual bool askPlayerYesNo(const QString& caption, const QString& text);
    public slots:
        virtual void focus(int x, int y);
    signals:
        void onKeyPress(QKeyEvent* event);
        void onRepaintStats(QStatsEvent* event);
        void onSquareDoubleClicked(int x, int y);
        void onActionPointChanged(int x, int y);
};

#endif
