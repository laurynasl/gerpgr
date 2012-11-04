#ifndef OBJECT_CLASS_HPP_
#define OBJECT_CLASS_HPP_

#include "../parser/parser.hpp"
#include "masks.hpp"
#include <QString>
#include <QVariantMap>

class ObjectClass{
    public:
        QString name;
        QVariantMap options;
        
        ObjectClass();
        ObjectClass(const ObjectClass& other);
        void load(Parser &parser);
        void print() const;
};

#endif
