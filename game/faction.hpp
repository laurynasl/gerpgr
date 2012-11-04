#ifndef FACTION_HPP_
#define FACTION_HPP_

#include <QString>
#include <QMap>
#include "../parser/parser.hpp"
#include <iostream>
#include "leader.hpp"

using namespace std;

/** \brief Group of people
 * 
 * People is very generalized term here.
 * 
 * \todo I need to think more about this. Every man can belong to several factions,
 * so if I know just how big every faction is, how to count all people? OK, faction
 * is not always the same as race, only for biggest factions, like civilians.
 * Smaller factions (like pirates) can consist of several races.
 * Should every faction have a leader? Change of a leader is a historical event.
 * 
 * Faction growth heavily depends on food in it's living area
 * 
 * So, a faction needs a leader...
 */
class Faction {
    protected:
        QString _name;
        /// initial relationships with previously unknown faction
        int _friendliness;
        /// relationships with other factions. positive number <=> positive relationships
        QMap<Faction*,int> _relationships;
        Leader _leader;
    public:
        Faction();
        virtual ~Faction();
        virtual void load(Parser &parser);
        virtual void save(ofstream& file);
};

#endif
