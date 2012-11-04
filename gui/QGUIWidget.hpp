#ifndef QGUIWIDGET_HPP_
#define QGUIWIDGET_HPP_

#include <QEvent>
#include <QWidget>
#include <QMenuBar>
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>
#include "../game/game.hpp"
#include "../engine/engine.hpp"
#include "../engine/qstatsevent.hpp"

class QGUIWidget : public QMainWindow {
    Q_OBJECT
    private:
    protected:
        QLabel *valueLife;
        QLabel *valueMana;
        QLabel *valueFatique;
        Engine *engine;
        QPushButton *newGameButton;
        QPushButton *saveButton;
        QPushButton *loadButton;
        QErrorMessage *errorMessage;
        Game *game;
        QWidget* centralWidget;
        
        QMenu* fileMenu;
        QMenu* viewMenu;
        
//         QAction* generateGameAction;
        QAction* squareSizeAction;
        
        virtual void keyPressEvent(QKeyEvent * event);
    public:
        QGUIWidget();
        ~QGUIWidget();
        Engine *getEngineWidget() { return engine; }
        void setGame(Game *_game);
        void connectSignalsToSlots();
    public slots:
        void newGameClicked();
        void loadGameClicked();
        void saveGameClicked();
        void repaintStats(QStatsEvent * event);
        void changeSquareSize();
//         void generateGame();
    signals:
        void signalNewGame();
        void onKeyPress(QKeyEvent * event);
};

#endif /*QGUIWIDGET_HPP_*/
