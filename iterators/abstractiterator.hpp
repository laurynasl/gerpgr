#ifndef ABSTRACT_ITERATOR_HPP_
#define ABSTRACT_ITERATOR_HPP_


/**
 * 
 * for (AbstractIterator i; !i; i++);
 */
class AbstractIterator {
    public:
        virtual bool operator !() = 0;
        virtual void operator ++(int) = 0;
};

#endif
