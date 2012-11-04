#include "actionrandom.hpp"
#include "tables.hpp"
#include "maps.hpp"
#include "monsters.hpp"
#include <assert.h>

ActionRandom:: ActionRandom(Parser& parser) {
    load(parser);
}

bool ActionRandom:: run(Monster& _monster) {
    QList<QIntsPair> points = coordinates.getPoints();
    GameMap* map = Maps::nameToMap(mapName);
    assert(map != 0);
    for (int i=0; i<monstersCount; i++) {
        int x, y;
        bool inserted = false;
        Monster* monster = new Monster();
        monster->random(monsterType);
        while (cutRandomCoordinate(points, x, y)) {
            if (map->canAcceptMonsterAt(x, y, *monster)) {
                monster->setX(x);
                monster->setY(y);
                monster->setMapName(mapName);
                monster->finishedLoading();
                map->insertMonster(monster);
                Monsters::add(monster);
                inserted = true;
                cout << "randinsert " << x << " " << y << endl;
                break;
            }
        }
        if (!inserted) {
            delete monster;
            break;
        }
    }
    return true;
}

void ActionRandom:: load(Parser &parser) {
    //cout << "loading random action" << endl;
    TokenType token = (TokenType) parser.nextToken();
    if ((token == tktMonster) || (token == tktMonsters)) {
        monstersCount = 1;
        if (token == tktMonsters) {
            parser >> monstersCount;
        }
        QString monsterName;
        parser >> monsterName;
        monsterType = Tables::nameToMonster(monsterName);
        coordinates.load(parser);
        parser >> mapName;
    }
    else {
        throw EException("actionrandom.cpp: UTA: \"random\"");
    }
}

void ActionRandom:: save(ofstream& file) {
    file << "        random monsters " << monstersCount;
    file << " \"" << Tables::monsterToClass(monsterType).name.toStdString() << "\" ";
    coordinates.save(file);
    file << "\"" << mapName.toStdString() << "\"" << endl;
}

int ActionRandom:: test(bool verbose) const {
    int result = 0;
    if (verbose) {
        cout << "ActionRandom::test not yet implemented" << endl;
    }
    return result;
}
