#include "backgroundclass.hpp"
#include "tables.hpp"

#include <iostream>

using namespace std;

BackgroundClass:: BackgroundClass() {
    flags = 0;
    height = 0;
}

BackgroundClass:: BackgroundClass(const BackgroundClass& other) {
    name = other.name;
    flags = other.flags;
    height = other.height;
    type = other.type;
}

void BackgroundClass:: load(Parser& parser) {
    parser >> name;
    TokenType token;
    do {
        token = (TokenType) parser.nextToken();
        switch (token) {
            case tktObstacle: {
                flags |= bkgFlgObstacle;
                break;
            }
            case tktBulletproof: {
                flags |= bkgFlgBulletproof;
                break;
            }
            case tktVisionproof: {
                flags |= bkgFlgVisionproof;
                break;
            }
            case tktMainmap: {
                flags |= bkgFlgForMainmap;
                break;
            }
            case tktUnderground: {
                flags |= bkgFlgUnderground;
                break;
            }
            case tktHeight: {
                parser >> height;
                break;
            }
            case tktType: {
                parser >> type;
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                throw EException(QString("backgroundclass.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
            }
        }
    } while (token != tktEnd);
}

bool BackgroundClass:: isObstacle() {
    return (flags & bkgFlgObstacle) != 0;
}

bool BackgroundClass:: isBulletproof() {
    return (flags & bkgFlgBulletproof) != 0;
}

bool BackgroundClass:: isVisionproof() {
    return (flags & bkgFlgVisionproof) != 0;
}

bool BackgroundClass:: isForMainmap() {
    return (flags & bkgFlgForMainmap) != 0;
}

void BackgroundClass:: print() const {
    cout << "name=" << name.toStdString() << " flags=" << flags << " height=" << height << " type=" << type.toStdString() << endl;
}
