#ifndef MONSTER_CLASS_HPP_
#define MONSTER_CLASS_HPP_

#include "../parser/parser.hpp"
#include "masks.hpp"
#include <QString>
#include <QMap>
#include <QPair>

typedef QMap<int,QPair<float,float> > SkillRanges;

/** \brief Descriptor class for Monster
 */
class MonsterClass {
    public:
        QString name;
        int minAttributes[9];
        int maxAttributes[9];
        //int minLevel;
        //int maxLevel;
        SkillRanges skillRanges;
        int type; //MonsterType
        
        MonsterClass();
        MonsterClass(const MonsterClass& other);
        void load(Parser &parser, int type);
        void print();
};

#endif
