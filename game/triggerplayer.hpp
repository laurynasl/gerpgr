#ifndef TRIGGER_PLAYER_HPP_
#define TRIGGER_PLAYER_HPP_

#include "gamemap.hpp"

/** \brief Trigger which is passed only by Player
 */
class TriggerPlayer : public Trigger {
    protected:
        void custLoad(Parser& parser);
        void custSave(ofstream& file) const;
    public:
        TriggerPlayer();
        ~TriggerPlayer();
        bool canActivate(Monster& monster);
        QString name() const;
};

#endif
