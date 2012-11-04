#include "QGUIWidget.hpp"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QFile>
#include <QInputDialog>
#include "../engine/engine.hpp"
#include "../game/game.hpp"
#include "../game/exception.hpp"
#include <assert.h>

#include <iostream>
using namespace std;

QGUIWidget:: QGUIWidget () {
    centralWidget = new QWidget();
    centralWidget->setGeometry(0, 0, 500, 500);
    setCentralWidget(centralWidget);
    
    setGeometry(0, 0, 500, 500);
    
    //main grid
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    
    //buttons layout
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    gridLayout->addLayout(buttonsLayout, 0, 0);
    
    //buttons
    newGameButton = new QPushButton("&New Game");
    newGameButton->setFocusPolicy(Qt::NoFocus);
    buttonsLayout->addWidget(newGameButton);
    saveButton = new QPushButton("&Save");
    saveButton->setFocusPolicy(Qt::NoFocus);
    buttonsLayout->addWidget(saveButton);
    loadButton = new QPushButton("&Load");
    loadButton->setFocusPolicy(Qt::NoFocus);
    buttonsLayout->addWidget(loadButton);
    
    //properties grid (name/value, name/value, ...)
    QGridLayout *propertiesGrid = new QGridLayout();
    gridLayout->addLayout(propertiesGrid, 1, 0);
    
    //properties QLabels
    //    life:
    QLabel *labelLife = new QLabel("Life");
    propertiesGrid->addWidget(labelLife, 0, 0);
    valueLife = new QLabel("10/10");
    propertiesGrid->addWidget(valueLife, 0, 1);
    //    mana:
    QLabel *labelMana = new QLabel("Mana");
    propertiesGrid->addWidget(labelMana, 1, 0);
    valueMana = new QLabel("5/5");
    propertiesGrid->addWidget(valueMana, 1, 1);
    //    fatique:
    QLabel *labelFatique = new QLabel("Fatique");
    propertiesGrid->addWidget(labelFatique, 2, 0);
    valueFatique = new QLabel("8/8");
    propertiesGrid->addWidget(valueFatique, 2, 1);
    
    //engine widget
    engine = new Engine();
    engine->setFocus();
//     engine->show();
    gridLayout->addWidget(engine, 0, 1, 2, 1);
    
    //setFocusProxy(engine);
    //engine->setFocusPolicy(Qt::ClickFocus);
    engine->show();
    
    //main grid stretch
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 7);
    
    //error message
    errorMessage = new QErrorMessage(centralWidget);
    
    // try to set up main menu
    fileMenu = menuBar()->addMenu("&File");
    {
//         generateGameAction = fileMenu->addAction("Generate game");
//         assert(connect(generateGameAction, SIGNAL(triggered()), this, SLOT(generateGame())));
    }
    viewMenu = menuBar()->addMenu("&View");
    {
        squareSizeAction = viewMenu->addAction("Change square size");
        assert(connect(squareSizeAction, SIGNAL(triggered()), this, SLOT(changeSquareSize())));
    }
}

QGUIWidget:: ~QGUIWidget () {
}

void QGUIWidget:: setGame(Game* _game) {
    game = _game;
}

void QGUIWidget:: newGameClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "", "data/maps");
    if (fileName.isEmpty()) return;
    try{
        game->load(fileName);
    }
    catch(QString &e) {
        errorMessage->showMessage(e + " (QString)");
    }
    catch(EException &e) {
        errorMessage->showMessage(e.message());
    }
    catch(...) {
        errorMessage->showMessage("Other error");
    }
}

void QGUIWidget:: loadGameClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "", "savegames");
    if (fileName.isEmpty()) return;
    try{
        game->load(fileName);
    }
    catch(QString &e) {
        errorMessage->showMessage(e + " (QString)");
    }
    catch(EException &e) {
        errorMessage->showMessage(e.message());
    }
    catch(...) {
        errorMessage->showMessage("Other error");
    }
}

void QGUIWidget:: saveGameClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "", "savegames");
    if (QFile::exists(fileName)) {
        QString backupName = fileName + "~";
        if (QFile::exists(backupName)) {
            if (!QFile::remove(backupName)) {
                throw QString("Unable to remove file %1").arg(backupName);
            }
        }
        if (!QFile::copy(fileName, backupName)) {
            throw QString("Unable to backup file %1").arg(fileName);
        }
    }
    if (fileName.isEmpty()) return;
    try{
        game->save(fileName);
    }
    catch(QString &e) {
        errorMessage->showMessage(e + " (QString)");
    }
    catch(EException &e) {
        errorMessage->showMessage(e.message());
    }
    catch(...) {
        errorMessage->showMessage("Other error");
    }
}

void QGUIWidget:: repaintStats(QStatsEvent * event) {
    valueLife->setText(QString("%1/%2").arg(event->hp).arg(event->maxhp));
    valueFatique->setText(QString("%1/%2").arg(event->fp).arg(event->maxfp));
    valueMana->setText(QString("%1/%2").arg(event->mp).arg(event->maxmp));
    event->accept();
}
void QGUIWidget:: changeSquareSize() {
    bool ok;
    int size = QInputDialog::getInteger(this, "Change square size", "Select new square size", engine->getSquareSize(), 16, 128, 4, &ok);
    if (ok) {
        engine->setSquareSize(size);
    }
}

void QGUIWidget:: keyPressEvent(QKeyEvent * event) {
    emit onKeyPress(event);
}

void QGUIWidget:: connectSignalsToSlots() {
    assert(connect(newGameButton, SIGNAL(pressed()), this, SLOT(newGameClicked())));
    assert(connect(loadButton, SIGNAL(pressed()), this, SLOT(loadGameClicked())));
    assert(connect(saveButton, SIGNAL(pressed()), this, SLOT(saveGameClicked())));
    assert(connect(engine, SIGNAL(onKeyPress(QKeyEvent *)), game, SLOT(onKeyPress(QKeyEvent *))));
    assert(connect(this, SIGNAL(onKeyPress(QKeyEvent *)), game, SLOT(onKeyPress(QKeyEvent *))));
    assert(connect(engine, SIGNAL(onRepaintStats(QStatsEvent*)), this, SLOT(repaintStats(QStatsEvent*))));
    assert(connect(game, SIGNAL(repaintStats(QStatsEvent*)), this, SLOT(repaintStats(QStatsEvent*))));
    assert(connect(engine, SIGNAL(onSquareDoubleClicked(int,int)), game, SLOT(onSquareDoubleClicked(int,int))));
    
    assert(connect(engine, SIGNAL(onActionPointChanged(int,int)), game, SLOT(changeActionPoint(int,int))));
    assert(connect(engine->unlockDoorAction, SIGNAL(triggered()), game, SLOT(unlockDoor())));
    assert(connect(engine->lockDoorAction, SIGNAL(triggered()), game, SLOT(lockDoor())));
    assert(connect(engine->openDoorAction, SIGNAL(triggered()), game, SLOT(openDoor())));
    assert(connect(engine->closeDoorAction, SIGNAL(triggered()), game, SLOT(closeDoor())));
}
