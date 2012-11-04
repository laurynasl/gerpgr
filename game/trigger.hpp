#ifndef TRIGGER_HPP_
#define TRIGGER_HPP_

#include "abstractaction.hpp"
#include "coordinateslist.hpp"
#include "../parser/parser.hpp"
#include "monster.hpp"
#include <iostream>
#include <QList>
#include <QString>

using namespace std;

/** \brief Abstract class for all triggers
 */
class Trigger : public Testable {
    protected:
        QList<AbstractAction*> actions;
        CoordinatesList coordinates;
        int duration;
        virtual void custLoad(Parser& parser) = 0;
        virtual void custSave(ofstream& file) const = 0;
        void saveActions(ofstream& file) const;
        void loadActions(Parser& parser);
    public:
        Trigger();
        virtual ~Trigger();
        void load(Parser& parser);
        void save(ofstream& file) const;
        // called, when monster steps on square[x,y]
        bool activate(Monster& monster);
        virtual bool canActivate(Monster& monster) = 0;
        QList<QIntsPair> getPoints() const; 
        virtual QString name() const = 0;
        int test(bool verbose) const;
};

#endif
