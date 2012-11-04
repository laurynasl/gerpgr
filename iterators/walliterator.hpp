#ifndef WALL_ITERATOR_HPP_
#define WALL_ITERATOR_HPP_

#include "abstractiterator.hpp"

/**
 * 
 * Let x and y be coordinates so that 0<=x\<width, 0<=y\<height.
 * Then if x==0 or x==(width-1) or y==0 or y==(height-1), it is a wall.
 * WallIterator iterates these walls.
 */
class WallIterator : public AbstractIterator {
    protected:
        int _baseX, _baseY, _x, _y, _w, _h, _phase;
    public:
        WallIterator(int baseX, int baseY, int width, int height);
        bool operator !();
        void operator ++(int);
        int x();
        int y();
};

#endif
