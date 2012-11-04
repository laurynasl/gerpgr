#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <QList>
#include <QString>
#include "faction.hpp"
#include "ruin.hpp"

class Mountain {
    public:
        int z;
        
        Mountain();
        Mountain(const Mountain& other);
};

class HeightFlag {
    public:
        int x;
        int y;
        int z; /// how many meters above sea level (or below if negative)
        int range; /// area of effect
        
        HeightFlag();
        HeightFlag(const HeightFlag& other);
        void load(Parser& parser);
        void save(ofstream& file) const;
        void randomize(int width, int height, int minZ, int maxZ, int minRange, int maxRange);
};

/** \brief World contains objects which are to be used to generate maps
 * 
 * Height map can be used to generate mainmap, to fill negative height areas with water.
 * So, there should be stored mountains, rivers, forests, swamps, vulcanos, lakes, seas, oceans, villages, roads, ruins, resources
 */
class World {
    protected:
        QString _name;
        int _width, _height;
        QList<HeightFlag> _heightFlags;
        QVector<QVector<float> > _heightMap;
        int _initialZ; /// initial height for _heightMap
        
        //objects
        QList<Ruin> _ruins;
        
        void generateHeightMap();
        void clear();
    public:
        World();
        virtual ~World();
        void load(Parser& parser, bool generate=false);
        void save(ofstream& file) const;
        const QVector<QVector<float> >& heightMap() const;
        const QList<Ruin> ruins() const;
        const QString name() const;
        const int width() const;
        const int height() const;
};

#endif
