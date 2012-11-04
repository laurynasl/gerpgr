#ifndef MONSTERS_HPP_
#define MONSTERS_HPP_

#include "monster.hpp"
#include <QSet>

class Monsters {
    protected:
        static Monsters* instance;
        QSet<Monster*> monsters;
        
    public:
        Monsters();
        ~Monsters();
        static Monsters& getInstance();
        static void add(Monster* monster);
        static bool remove(Monster* monster);
        static void clear();
        static QSet<Monster*>& getAllMonsters();
};

#endif
