#include <QApplication>
#include <QMainWindow>

#include <QApplication>
#include "gui/QGUIWidget.hpp"
#include "engine/engine.hpp"
#include "game/game.hpp"
#include "game/maps.hpp"
#include "game/tables.hpp"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{
    
    srand(time(0));//preparing random number generator
    QApplication app(argc, argv);
    QErrorMessage* errorMessage = new QErrorMessage(0);
    try {
        QGUIWidget widget;
        Game game;
        game.setEngine(widget.getEngineWidget());
        widget.setGame(&game);
        widget.connectSignalsToSlots();
        widget.show();
        app.exec();
        //cout << "after app.exec()" << endl;
    }
    catch(QString &e) {
        errorMessage->showMessage(e + " (QString)");
        cerr << e.toStdString() << " (QString)" << endl;
    }
    catch(EException &e) {
        errorMessage->showMessage(e.message());
        cerr << e.message().toStdString() << " (QString)" << endl;
    }
    catch(...) {
        errorMessage->showMessage("Other error");
        cerr << "Other error" << endl;
    }
    //cout << "main.cpp: delete errorMessage" << endl;
    delete errorMessage;
    //cout << "main.cpp: delete Tables" << endl;
    // destroy singletons
    delete &(Tables::getInstance());
    //cout << "main.cpp: delete Maps" << endl;
    delete &(Maps::getInstance());
    //cout << "main.cpp: delete all" << endl;
}
