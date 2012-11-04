#include "worlds.hpp"
#include <assert.h>

Worlds* Worlds::instance = 0;

void Worlds:: init() {
}

Worlds:: Worlds() {
}

Worlds:: ~Worlds() {
    clear();
}

Worlds& Worlds:: getInstance() {
    if (!instance) {
        instance = new Worlds();
        instance->init();
    }
    return *instance;
}

void Worlds:: add(World* world) {
    assert(world != 0);
    getInstance().worlds[world->name()] = world;
    cout << "added world " << world->name().toStdString() << endl;
}

void Worlds:: remove(World* world) {
    assert(world != 0);
    getInstance().worlds.remove(world->name());
}

World* Worlds:: nameToWorld(const QString name) {
    if (getInstance().worlds.contains(name)) {
        return getInstance().worlds[name];
    }
    else {
        return 0;
    }
}

void Worlds:: clear() {
    foreach (World* world, getInstance().worlds) {
        assert(world != 0);
        delete world;
    }
    getInstance().worlds.clear();
}

const QMap<QString,World*>& Worlds:: getAllWorlds() {
    return getInstance().worlds;
}
