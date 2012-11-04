#ifndef ABSTRACT_ACTION_HPP_
#define ABSTRACT_ACTION_HPP_

#include "testable.hpp"
#include "monster.hpp"
#include "../parser/parser.hpp"
#include <fstream>

using namespace std;

/** \brief Abstract class for all trigger actions
 */
class AbstractAction : public Testable {
    public:
        virtual ~AbstractAction() {}
        virtual bool run(Monster& monster) = 0;
        virtual void load(Parser& parser) = 0;
        virtual void save(ofstream& file) = 0;
};

#endif
