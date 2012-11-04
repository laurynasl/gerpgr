#include "actiontravel.hpp"
#include "maps.hpp"
#include <assert.h>


ActionTravel:: ActionTravel(Parser& parser) {
    load(parser);
}

bool ActionTravel:: run(Monster& monster) {
    QList<QIntsPair> points = coordinates.getPoints();
    GameMap* targetMap = Maps::nameToMap(mapName);
    assert(targetMap != 0);
    int x, y;
    while (cutRandomCoordinate(points, x, y)) {
        if (targetMap->canAcceptMonsterAt(x, y, monster)) {
            monster.teleport(mapName, x, y);
            return true;
        }
    }
    return false;
}

void ActionTravel:: load(Parser& parser) {
    parser >> mapName;
    coordinates.load(parser);
}

void ActionTravel:: save(ofstream& file) {
    file << "        travel \"";
    file << mapName.toStdString() << "\" ";
    coordinates.save(file);
    file << endl;
}

int ActionTravel:: test(bool verbose) const {
    int result = 0;
    if (!Maps::nameToMap(mapName)) {
        result = max(result, 2);
        if (verbose) {
            cout << "ActionTravel: map does not exist:" << mapName.toStdString() << endl;
        }
        return result;
    }
    if (verbose) {
        cout << "ActionTravel: not implemented test for squares check" << endl;
    }
    return result;
}
