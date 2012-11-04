#include "attackdescriptor.hpp"
#include "masks.hpp"
#include "tables.hpp"

#include <iostream>

using namespace std;

AttackDescriptor:: AttackDescriptor() {
    attack = 0;
    hCount = 1;
    hMaximum = 4;
    hDamage = 0;
    fCount = 1;
    fMaximum = 4;
    fDamage = 0;
    minStrength = 0;
}

AttackDescriptor:: AttackDescriptor(const AttackDescriptor& other) {
    attack = other.attack;
    hCount = other.hCount;
    hMaximum = other.hMaximum;
    hDamage = other.hDamage;
    fCount = other.fCount;
    fMaximum = other.fMaximum;
    fDamage = other.fDamage;
    minStrength = other.minStrength;
    skills = other.skills;
}

void AttackDescriptor:: load(Parser& parser) {
    TokenType token;
    skills.clear();
    do {
        token = (TokenType) parser.nextToken();
        switch (token) {
            case tktAttack: {
                parser >> attack;
                parser >> hCount >> hMaximum >> hDamage;
                parser >> fCount >> fMaximum >> fDamage;
                break;
            }
            case tktMinstrength: {
                parser >> minStrength;
                break;
            }
            case tktSkill: {
                QString name;
                parser >> name;
                skills.append(Tables::nameToSkill(name));
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                throw EException(QString("attackdescriptor.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
            }
        }
    } while (token != tktEnd);
}

void AttackDescriptor:: print() const {
    cout << "        attack " << attack << ", " << hCount << " " << hMaximum << " " << hDamage << ", " << fCount << " " << fMaximum << " " << fDamage << endl;
    cout << "        minstrength " << minStrength << endl;
    foreach (int skill, skills) {
        cout << "        skill \"" << Tables::skillToName(skill).toStdString() << "\"" << endl;
    }
}
