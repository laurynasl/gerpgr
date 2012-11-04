#ifndef ACTION_RANDOM_HPP_
#define ACTION_RANDOM_HPP_

#include "gamemap.hpp"
#include "coordinateslist.hpp"

/** \brief Action, randomly generating monsters at specified map
 */
class ActionRandom : public AbstractAction {
    protected:
        int monstersCount;
        CoordinatesList coordinates;
        int monsterType;
        QString mapName;
    public:
        ActionRandom(Parser& parser);
        bool run(Monster& _monster);
        void load(Parser& parser);
        void save(ofstream& file);
        int test(bool verbose) const;
};

#endif
