#ifndef BACKGROUND_CLASS_HPP_
#define BACKGROUND_CLASS_HPP_

#include "../parser/parser.hpp"
#include <QString>

const int bkgFlgObstacle =      0x00000001;
const int bkgFlgBulletproof =   0x00000002;
const int bkgFlgVisionproof =   0x00000004;
const int bkgFlgForMainmap =    0x00000008;
const int bkgFlgUnderground =   0x00000010;

/** \brief Descriptor class for MapSquare::background
 */
class BackgroundClass {
    public:
        QString name;
        int flags;
        int height;
        QString type;
        
        BackgroundClass();
        BackgroundClass(const BackgroundClass& other);
        void load(Parser& parser);
        bool isObstacle();
        bool isBulletproof();
        bool isVisionproof();
        bool isForMainmap();
        void print() const;
};

#endif
