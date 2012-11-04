#include "walliterator.hpp"
#include <iostream>

using namespace std;

WallIterator:: WallIterator(int baseX, int baseY, int width, int height) {
    _baseX = baseX;
    _baseY = baseY;
    _x = 0;
    _y = 0;
    _w = width;
    _h = height;
    _phase = 0;
}

bool WallIterator:: operator !() {
    return _phase != 4;
}

void WallIterator:: operator ++(int) {
    switch (_phase) {
        case 0: {
            _x++;
            if (_x == (_w - 1)) {
                _phase++;
            }
            break;
        }
        case 1: {
            _y++;
            if (_y == (_h - 1)) {
                _phase++;
            }
            break;
        }
        case 2: {
            _x--;
            if (_x == 0) {
                _phase++;
            }
            break;
        }
        case 3: {
            _y--;
            if (_y == 0) {
                _phase++;
            }
            break;
        }
        case 4:
        default: {
            cerr << "walliteratot.cpp: operator++: _phase == " << _phase << endl;
        }
    }
}

int WallIterator:: x() {
    return _baseX + _x;
}

int WallIterator:: y() {
    return _baseY + _y;
}
