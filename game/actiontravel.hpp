#ifndef ACTION_TRAVEL_HPP_
#define ACTION_TRAVEL_HPP_

#include "gamemap.hpp"
#include "coordinateslist.hpp"

/** \brief Teleportation between maps action
 */
class ActionTravel : public AbstractAction {
    protected:
        QString mapName;
        CoordinatesList coordinates;
    public:
        ActionTravel(Parser& parser);
        bool run(Monster& monster);
        void load(Parser& parser);
        void save(ofstream& file);
        int test(bool verbose) const;
};

#endif
