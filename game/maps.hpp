#ifndef MAPS_HPP_
#define MAPS_HPP_

#include "gamemap.hpp"
#include <QMap>
#include <QString>

/** \brief singleton providing maps list
 */
class Maps {
    protected:
        static Maps* instance;
        QMap<QString,GameMap*> maps;
        
        void init();
    public:
        Maps();
        ~Maps();
        static Maps& getInstance();
        static void add(GameMap* map);
        static void remove(GameMap* map);
        static GameMap* nameToMap(const QString name);
        static void clear();
        static const QMap<QString,GameMap*>& getAllMaps();
};

#endif
