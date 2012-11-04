#include "monsterclass.hpp"
#include "masks.hpp"
#include "tables.hpp"
#include <iostream>

using namespace std;

MonsterClass:: MonsterClass() {
    name = "SomeMonster";
    for (int i=0; i<9; i++) {
        minAttributes[i] = 10;
        maxAttributes[i] = 10;
    }
    //minLevel = 1;
    //maxLevel = 1;
    //type = monsterTypeMask;
}

MonsterClass:: MonsterClass(const MonsterClass& other) {
    name = other.name;
    //minLevel = other.minLevel;
    //maxLevel = other.maxLevel;
    for (int i=0; i<9; i++) {
        minAttributes[i] = other.minAttributes[i];
        maxAttributes[i] = other.maxAttributes[i];
    }
    skillRanges = other.skillRanges;
}

void MonsterClass:: load(Parser &parser, int type) {
    this->type = type;
    parser >> name;
    TokenType token;
    do {
        token = (TokenType) parser.nextToken();
        switch (token) {
            case tktStrength: {
                parser >> minAttributes[0] >> maxAttributes[0];
                break;
            }
            case tktConstitution: {
                parser >> minAttributes[1] >> maxAttributes[1];
                break;
            }
            case tktDexterity: {
                parser >> minAttributes[2] >> maxAttributes[2];
                break;
            }
            case tktIntelligence: {
                parser >> minAttributes[3] >> maxAttributes[3];
                break;
            }
            case tktCharisma: {
                parser >> minAttributes[4] >> maxAttributes[4];
                break;
            }
            case tktWillpower: {
                parser >> minAttributes[5] >> maxAttributes[5];
                break;
            }
            case tktLuck: {
                parser >> minAttributes[6] >> maxAttributes[6];
                break;
            }
            case tktPerception: {
                parser >> minAttributes[7] >> maxAttributes[7];
                break;
            }
            case tktBeauty: {
                parser >> minAttributes[8] >> maxAttributes[8];
                break;
            }
            case tktSkill: {
                QString skillName;
                float min, max;
                parser >> skillName >> min >> max;
                int skill = Tables::nameToSkill(skillName);
                skillRanges.insert(skill, QPair<float,float>(min, max));
            }
            case tktEnd: {
                break;
            }
            default: {
                cout << "MonsterClass default " << token << endl;
                throw EException(QString("monsterclass.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
            }
        }
    } while (token != tktEnd);
}

void MonsterClass:: print() {
    cout << name.toStdString().c_str() << endl;
    //cout << "\tlevel " << minLevel << ' ' << maxLevel << endl;
    for (int i=0; i<9; i++) {
        cout << "\t" << minAttributes[i] << ' ' << maxAttributes[i] << endl;
    }
}
