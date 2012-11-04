#ifndef LINE_ITERATOR_HPP_
#define LINE_ITERATOR_HPP_

#include "abstractiterator.hpp"

class LineIterator : public AbstractIterator {
    protected:
        int _x1, _x2, _y1, _y2, _param, _max;
        int _lastx, _lasty, _x, _y;
    public:
        LineIterator(int x1, int y1, int x2, int y2);
        virtual bool operator !();
        virtual void operator ++(int);
        int x();
        int y();
};

#endif
