#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "monster.hpp"
#include "../parser/parser.hpp"
#include <Qt>

/** \brief The same Monster, but represents human(?) who plays this game
 */
class Player : public Monster {
    Q_OBJECT
    public:
        virtual void load(Parser &parser);
        virtual void save(ofstream& file) const;
        virtual bool isMonster();
        virtual bool teleport(const QString& mapName, int x, int y);
        virtual void moveBy(int dx, int dy);
        virtual void random(int monsterType);
        virtual void resetMaximums();
        virtual void naturallyRegenerate();
        virtual void damaged();
    signals:
        void playerMoved();
        void healthChange();
};

#endif
