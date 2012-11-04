#ifndef TRIGGER_QUESTION_HPP_
#define TRIGGER_QUESTION_HPP_

#include "../parser/parser.hpp"
#include "gamemap.hpp"

/** \brief Another Player-only Trigger, asking Yes/No question
 */
class TriggerQuestion : public Trigger{
    public:
        QString message;
        
        TriggerQuestion();
        ~TriggerQuestion();
        void custLoad(Parser& parser);
        void custSave(ofstream& file) const;
        bool canActivate(Monster& monster);
        QString name() const;
};

#endif
