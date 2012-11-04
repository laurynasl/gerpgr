#ifndef MONSTER_HPP_
#define MONSTER_HPP_

#include "masks.hpp"
#include "../parser/parser.hpp"
#include "monsterclass.hpp"
#include "item.hpp"
#include "itemslist.hpp"
#include "attackdescriptor.hpp"
#include "skill.hpp"
#include <iostream>
#include <Qt>
#include <QObject>
#include <QMap>

using namespace std;

enum AttributeType {
    Strength,
    Constitution,
    Dexterity,
    Intelligence,
    Charisma,
    Willpower,
    Luck,
    Perception,
    Beauty
};

/** \brief General class for any person, which can move, fight, communicate...
 */
class Monster : public QObject {
    Q_OBJECT
    protected:
        QString mapName;
        int x;
        int y;
        
        int monsterType;
        int attributes[9];
        //health state
        int maxhp;
        int hp;
        int maxfp;
        int fp;
        int maxmp;
        int mp;
        
        int hpfrac;
        int fpfrac;
        int mpfrac;
        
        // all skills
        QMap<int, Skill> skills;
        
        
        // AI fields
        Monster * target;
        /////////////////
        
        bool loaded;
    public:
        QString name;
        // items
        ItemsList items;
        QMap<int,Item> equipment;
        // speed system
        int energy;
        
        Monster();
        virtual ~Monster();
        void clear();
        virtual void load(Parser& parser);
        virtual void save(ofstream& file) const;
        virtual void random(int monsterType);
        
        //void moveTo(int x, int y);
        void loadHealthState(Parser& parser);
        int getMonsterType();
        virtual void resetMaximums();
        void energyMove();
        // regenerates energy, hp, fp, mp
        virtual void naturallyRegenerate();
        bool isAlive();
        bool isConcious();
        
        // chance-to-hit or similar chance in percents
        int chance(double ratingDiff);
        int rollDice(int multiplier, int maximum);
        int dv() const;
        void meleeAttack(Monster& target);
        void weaponAttack(Monster& target, AttackDescriptor& attDesc, QList<int> attackSkills);
        bool canEquipAs(int itemID, int equipSlot) const;
        void equip(int itemID, int equipSlot);
        QPair<bool,QString> isSlotEquippable(int equipSlot);
        AttackDescriptor unarmedAttackDescriptor() const;
        int squareRangeTo(Monster& target);
        int squareRangeTo(int x, int y);
        ItemsList takeAllItems();
        
        void print();
        
        // skills abstraction
        bool increaseSkill(int skill, float value);
        int skillLevel(int skill) const;
        virtual bool isMonster();
        QString getMapName() const;
        void setMapName(const QString mapName);
        int getMonsterType() const;
        void setMonsterType(int type);
        
        // bits of improved architecture:
        // returns true if successfull
        virtual bool teleport(const QString& mapName, int x, int y);
        virtual void moveBy(int dx, int dy);
        void aiMove();
        virtual void damaged();
        int getX() const;
        int getY() const;
        void setX(int x);
        void setY(int y);
        int getHp() const;
        int getFp() const;
        int getMp() const;
        int getMaxHp() const;
        int getMaxFp() const;
        int getMaxMp() const;
        void finishedLoading();
        
        // inventory...
        bool hasKey(QString code) const;
    protected:
        void pSave(ofstream& file, const char* paddingSpaces) const;
        void pLoad(Parser& parser);
        void repaint();
    signals:
        void mapChanged();
};

#endif
