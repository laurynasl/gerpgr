#include "coordinateslist.hpp"
#include "tables.hpp"
#include <stdlib.h>

Coordinate:: Coordinate() {
}

Coordinate:: Coordinate(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Coordinate:: Coordinate(const Coordinate& other) {
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;
}

void Coordinate:: loadAt(Parser& parser) {
    parser >> x >> y;
    w = 1;
    h = 1;
}

void Coordinate:: loadIn(Parser& parser) {
    parser >> x >> y >> w >> h;
}

const void Coordinate:: save(ofstream& file) const {
    if ((w == 1) && (h == 1)) {
        file << "at " << x << " " << y << " ";
    }
    else {
        file << "in " << x << " " << y << " " << w << " " << h << " ";
    }
}

bool Coordinate:: operator <= (const Coordinate& other) const {
    return ((x >= other.x) || (y >= other.y) || ((x+w) <= (other.x+other.w)) || ((y+h) <= (other.y+other.h)));
}

/************************ CoordinatesList ************************/

CoordinatesList:: CoordinatesList() {
}

CoordinatesList:: CoordinatesList(const CoordinatesList& other) {
    list = other.list;
}

void CoordinatesList:: append(int x, int y, int w, int h) {
    Coordinate coordinate(x, y, w, h);
    append(coordinate);
}

void CoordinatesList:: append(const Coordinate& coordinate) {
    list.append(coordinate);
}

void CoordinatesList:: loadFull(Parser& parser) {
    list.clear();
    int token;
    do {
        token = parser.nextToken();
        switch (token) {
            case tktEnd: {
                break;
            }
            case tktAt: {
                Coordinate coordinate;
                coordinate.loadAt(parser);
                list.append(coordinate);
                break;
            }
            case tktIn: {
                Coordinate coordinate;
                coordinate.loadIn(parser);
                list.append(coordinate);
                break;
            }
            default: {
                throw EException(QString("coordinateslist.cpp: CoordinatesList::loadFull(): Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
            }
        }
    } while (token != tktEnd);
}

void CoordinatesList:: loadAt(Parser& parser) {
    list.clear();
    Coordinate coordinate;
    coordinate.loadAt(parser);
    list.append(coordinate);
}

void CoordinatesList:: loadIn(Parser& parser) {
    list.clear();
    Coordinate coordinate;
    coordinate.loadIn(parser);
    list.append(coordinate);
}

void CoordinatesList:: load(Parser& parser) {
    list.clear();
    int token;
    token = parser.nextToken();
    switch (token) {
        case tktAt: {
            loadAt(parser);
            break;
        }
        case tktIn: {
            loadIn(parser);
            break;
        }
        case tktCoordinates: {
            loadFull(parser);
            break;
        }
        case tktEnd: {
            break;
        }
        default: {
            throw EException(QString("coordinateslist.cpp: CoordinatesList::load(): Unexpected token \"%1\"").arg(Tables::tokenToName(token)));
        }
    }
}

const void CoordinatesList:: save(ofstream& file) const {
    if (list.count() == 1) {
        list[0].save(file);
    }
    else {
        file << "coordinates ";
        foreach (const Coordinate& coordinate, list)
            coordinate.save(file);
        file << "end ";
    }
}
QList<Coordinate> CoordinatesList:: getCoordinates() const {
    return list;
}

QList<QIntsPair> CoordinatesList:: getPoints() const {
    QList<QPair<int,int> > points;
    foreach (const Coordinate& coordinate, list) {
        for (int i=0; i<coordinate.w; i++) {
            for (int j=0; j<coordinate.h; j++) {
                points.append(QIntsPair(i+coordinate.x, j+coordinate.y));
            }
        }
    }
    return points;
}

bool cutRandomCoordinate(QList<QIntsPair>& list, int& x, int& y) {
    if (list.empty()) {
        return false;
    }
    int index = rand() % list.count();
    QIntsPair pair = list.takeAt(index);
    x = pair.first;
    y = pair.second;
    return true;
}

bool CoordinatesList:: operator <= (const Coordinate& other) const {
    bool result = true;
    foreach (const Coordinate& coordinate, list) {
        result = result and (coordinate <= other);
    }
    return result;
}
