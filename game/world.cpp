#include "world.hpp"
#include "tables.hpp"
#include "worlds.hpp"
#include "coordinateslist.hpp"
#include <math.h>
#include <fstream>

using namespace std;

#define RUINS_COUNT 1

/*************** Mountain **************/

Mountain:: Mountain() {
}

Mountain:: Mountain(const Mountain& other) {
    z = other.z;
}

/*************** HeightFlag **************/

HeightFlag:: HeightFlag() {
}

HeightFlag:: HeightFlag(const HeightFlag& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    range = other.range;
}

void HeightFlag:: load(Parser& parser) {
    parser >> x >> y >> z >> range;
}

void HeightFlag:: save(ofstream& file) const {
    file << "    height " << x << " " << y << " " << z << " " << range << endl;
}

void HeightFlag:: randomize(int width, int height, int minZ, int maxZ, int minRange, int maxRange) {
    x = rand() % width;
    y = rand() % height;
    z = rand() % (maxZ - minZ) + minZ;
    range = rand() % (maxRange - minRange) + minRange;
}

/*************** World **************/

void World:: generateHeightMap() {
    _heightMap.resize(_width);
    for (int i=0; i<_width; i++) {
        _heightMap[i].resize(_height);
        for (int j=0; j<_height; j++) {
            _heightMap[i][j] = _initialZ;
        }
    }
    foreach (const HeightFlag& flag, _heightFlags) {
        for (int i = -flag.range; i <= flag.range; i++) {
            for (int j = -flag.range; j <= flag.range; j++) {
                int x = flag.x + i;
                int y = flag.y + j;
                if (x < 0 || x >= _width) continue;
                if (y < 0 || y >= _height) continue;
                float range = sqrt((float) i * i + j * j);
                if (range > flag.range) continue;
                float dz = (flag.range - range) / flag.range * flag.z;
                _heightMap[x][y] += dz;
            }
        }
    }
}

void World:: clear() {
    _heightFlags.clear();
}

World:: World() {
    _initialZ = -1000; // everything is deep in water first...
}

World:: ~World() {
}

void World:: load(Parser& parser, bool generate) {
    int token;
    Worlds::remove(this);
    parser >> _name;
    Worlds::add(this);
    if (generate) {
        int minWidth, maxWidth, minHeight, maxHeight;
        parser >> minWidth >> maxWidth >> minHeight >> maxHeight;
        _width = minWidth + rand() % (maxWidth - minWidth + 1);
        _height = minHeight + rand() % (maxHeight - minHeight + 1);
    }
    else {
        parser >> _width >> _height;
    }
    parser >> _initialZ;
    do {
        token = parser.nextToken();
        switch(token) {
            case tktHeight: {
                HeightFlag flag;
                flag.load(parser);
                _heightFlags.append(flag);
                break;
            }
            case tktRandom: {
                int token = parser.nextToken();
                switch (token) {
                    case tktHeight: {
                        int count, minZ, maxZ, minRange, maxRange;
                        parser >> count >> minZ >> maxZ >> minRange >> maxRange;
                        for (int i=0; i<count; i++) {
                            HeightFlag flag;
                            flag.randomize(_width, _height, minZ, maxZ, minRange, maxRange);
                            _heightFlags.append(flag);
                        }
                        break;
                    }
                        default: {throw QString("world.cpp: load(): random: Unexpected token %1").arg(Tables::tokenToName(token));
                    }
                }
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                throw QString("world.cpp: load(): Unexpected token %1").arg(Tables::tokenToName(token));
            }
        }
    } while (token != tktEnd);
    generateHeightMap();
    CoordinatesList unusedCoordinates;
    unusedCoordinates.append(0, 0, _width, _height);
    QList<QIntsPair> unusedPairs = unusedCoordinates.getPoints();
    if (generate) {
        // add some ruins. let's say... 4
        int ruinsCount = 0;
        int x, y;
        while ((ruinsCount < RUINS_COUNT) && (cutRandomCoordinate(unusedPairs, x, y))) {
            /// in case water squares will be used for some objects, this must be changed:
            if (_heightMap[x][y] < 0) continue;
            Ruin ruin(QString("ruin%1").arg(ruinsCount+1), x, y);
            _ruins.append(ruin);
            ruinsCount++;
        }
    }
}

void World:: save(ofstream& file) const {
    file << "world \"" << _name.toStdString() << "\" " << _width << " " << _height << " " << _initialZ << endl;
    foreach (const HeightFlag& flag, _heightFlags) {
        flag.save(file);
    }
    file << "end" << endl;
}

const QVector<QVector<float> >& World:: heightMap() const {
    return _heightMap;
}

const QList<Ruin> World:: ruins() const {
    return _ruins;
}

const QString World:: name() const {
    return _name;
}

const int World:: width() const {
    return _width;
}

const int World:: height() const {
    return _height;
}
