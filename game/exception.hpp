#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <QString>

class EException {
    protected:
        QString  _message;
    public:
        EException(QString message) : _message(message) {}
        virtual ~EException() {}
        virtual QString message() { return _message; }
};




#endif 
