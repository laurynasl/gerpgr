#include "lineiterator.hpp"
#include <stdlib.h>
#include <math.h>

int max(int a, int b) {
    return a > b ? a : b;
}

int sign(int a) {
    return a < 0 ? -1 : 1;
}

LineIterator:: LineIterator(int x1, int y1, int x2, int y2) {
    _x1 = x1;
    _y1 = y1;
    _x2 = x2;
    _y2 = y2;
    _x = _x1;
    _y = _y1;
    _lastx = _x;
    _lasty = _y;
    _param = 0;
    _max = max(abs(x2-x1), abs(y2-y1));
}

bool LineIterator:: operator !() {
    return _param != _max;
}

void LineIterator:: operator ++(int) {
    _param++;
    _x = _x1 + (_param * (_x2 - _x1)) / _max;
    _y = _y1 + (_param * (_y2 - _y1)) / _max;
    int dx = _x - _lastx;
    int dy = _y - _lasty;
    int weight = dx * dx + dy * dy;
    if (weight > 1) {
        _param--;
        if (rand() % 2) {
            // _x goes back to _x1 by 1
            _x -= sign(_x2-_x1);
        }
        else {
            // _y goes back to _y1 by 1
            _y -= sign(_y2-_y1);
        }
    }
    _lastx = _x;
    _lasty = _y;
}

int LineIterator:: x() {
    return _x;
}

int LineIterator:: y() {
    return _y;
}
