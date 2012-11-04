#ifndef COORDINATES_LIST_HPP_
#define COORDINATES_LIST_HPP_

#include <QList>
#include <QPair>
#include "../parser/parser.hpp"
#include <fstream>

using namespace std;

typedef QPair <int, int> QIntsPair;

/** \brief Rectangular area. Can be single square
 */
class Coordinate {
    public:
        int x, y, w, h;
        Coordinate();
        Coordinate(int x, int y, int w = 1, int h = 1);
        Coordinate(const Coordinate& other);
        void loadAt(Parser& parser);
        void loadIn(Parser& parser);
        const void save(ofstream& file) const;
        bool operator <= (const Coordinate& other) const;
};

/** \brief Container and management class for Coordinate
 */
class CoordinatesList {
    protected:
        QList<Coordinate> list;
    public:
        CoordinatesList();
        CoordinatesList(const CoordinatesList& other);
        void append(int x, int y, int w = 1, int h = 1);
        void append(const Coordinate& coordinate);
        void loadFull(Parser& parser);
        void loadAt(Parser& parser);
        void loadIn(Parser& parser);
        void load(Parser& parser);
        const void save(ofstream& file) const;
        QList<Coordinate> getCoordinates() const;
        QList<QIntsPair> getPoints() const; 
        bool operator <= (const Coordinate& other) const;
};

bool cutRandomCoordinate(QList<QIntsPair>& list, int& x, int& y);

#endif
