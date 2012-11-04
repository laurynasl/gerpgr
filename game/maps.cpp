#include "maps.hpp"
#include <assert.h>

Maps* Maps::instance = 0;

void Maps:: init() {
}

Maps:: Maps() {
}

Maps:: ~Maps() {
    clear();
}

Maps& Maps:: getInstance() {
    if (!instance) {
        instance = new Maps();
        instance->init();
    }
    return *instance;
}

void Maps:: add(GameMap* map) {
    assert(map != 0);
    getInstance().maps[map->getName()] = map;
    cout << "added map " << map->getName().toStdString() << endl;
}

void Maps:: remove(GameMap* map) {
    assert(map != 0);
    getInstance().maps.remove(map->getName());
}

GameMap* Maps:: nameToMap(const QString name) {
    if (getInstance().maps.contains(name)) {
        return getInstance().maps[name];
    }
    else {
        return 0;
    }
}

void Maps:: clear() {
    foreach (GameMap* map, getInstance().maps) {
        assert(map != 0);
        delete map;
    }
    getInstance().maps.clear();
}

const QMap<QString,GameMap*>& Maps:: getAllMaps() {
    return getInstance().maps;
}
