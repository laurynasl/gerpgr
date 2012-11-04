#ifndef WORLDS_HPP_
#define WORLDS_HPP_

#include "world.hpp"
#include <QMap>
#include <QString>

class Worlds {
    protected:
        static Worlds* instance;
        QMap<QString,World*> worlds;
        
        void init();
    public:
        Worlds();
        ~Worlds();
        static Worlds& getInstance();
        static void add(World* world);
        static void remove(World* world);
        static World* nameToWorld(const QString name);
        static void clear();
        static const QMap<QString,World*>& getAllWorlds();
};

#endif
