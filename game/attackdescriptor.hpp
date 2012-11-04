#ifndef ATTACK_DESCRIPTOR_
#define ATTACK_DESCRIPTOR_

#include "../parser/parser.hpp"
#include <QList>

class AttackDescriptor {
    public:
        int attack;             // to-hit (attack) modifier
        int hCount, fCount;     // count of dices
        int hMaximum, fMaximum; // maximum dice value
        int hDamage, fDamage;   // damage modifier
        int minStrength;
        QList<int> skills;
        // (<attack>, <hCount>d<hMaximum>+<hDamage>) (health)
        // (<attack>, <fCount>d<fMaximum>+<fDamage>) (fatique)
        AttackDescriptor();
        AttackDescriptor(const AttackDescriptor& other);
        void load(Parser& parser);
        void print() const;
};

#endif
