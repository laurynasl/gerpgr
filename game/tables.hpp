#ifndef TABLES_HPP_
#define TABLES_HPP_

#include "monsterclass.hpp"
#include "itemclass.hpp"
#include "backgroundclass.hpp"
#include "objectclass.hpp"
#include "masks.hpp"
#include <QMap>
#include <QList>
#include <QString>
#include <limits.h>

class Tables {
    protected:
        QList<MonsterClass> _monsterClasses;
        QMap<QString,int> _namesToMonsters;
        QList<ItemClass> _itemClasses;
        QMap<QString,int> _namesToItems;
        QList<BackgroundClass> _backgroundClasses;
        QMap<QString,int> _namesToBackgrounds;
        QList<ObjectClass> _objectClasses;
        QMap<QString,int> _namesToObjects;
        
        QMap<QString,int> _namesToTokens;
        QMap<int,QString> _tokensToNames;
        QMap<QString,int> _namesToSkills;
        QList<QString> _skillsToNames;
        
        QList<QString> _keyCodes;
        
        bool waitingID; //very internal, used for token registration syntax
        QString lastRegToken;
        
        static Tables *instance;
        void init();
        Tables& operator +=(QString c);
        Tables& operator ,(QString c);
        Tables& operator ,(int number);
    public:
        Tables();
        ~Tables();
        static Tables& getInstance();
        
        
        static int nameToItem(const QString itemName);
        static const ItemClass& itemToClass(const int item);
        static int itemsCount();
        
        static int nameToMonster(const QString name);
        static MonsterClass& monsterToClass(const int monster);
        static int monstersCount();
        
        static int nameToBackground(const QString name);
        static BackgroundClass& backgroundToClass(const int background);
        static int backgroundsCount();
        static int search(const BackgroundClass& criteria, int checkedFlags=INT_MAX, bool debug = false);
        
        static int nameToObject(const QString name);
        static ObjectClass& objectToClass(const int object);
        static int objectsCount();
        static int searchObject(const QVariantMap& criteria);
        
        
        static int nameToToken(const QString name);
        static QString tokenToName(const int token);
        static bool tokenNameExists(QString name);
        
        static int nameToSkill(const QString name);
        static QString skillToName(const int skill);
        
        static void addKeyCode(QString code);
        static int keyCodesCount();
        static QString getRandomKeyCode();
};

#endif
