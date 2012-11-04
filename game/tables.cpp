#include "tables.hpp"
#include <iostream>
#include <assert.h>
#include <limits.h>

using namespace std;

Tables* Tables::instance = 0;

void Tables:: init() {
    *this +=
        "map",          tktMap,
        "tile",         tktTile,
        "tilesrange",   tktTilesRange,
        "player",       tktPlayer,
        "monster",      tktMonster,
        "monsters",     tktMonsters,
        "at",           tktAt,
        "random",       tktRandom,
        "in",           tktIn,
        "attributes",   tktAttributes,
        "health",       tktHealth,
        "name",         tktName,
        "end",          tktEnd,
        "trigger",      tktTrigger,
        "question",     tktQuestion,
        "onehanded",    tktOnehanded,
        "twohanded",    tktTwohanded,
        "thrown",       tktThrown,
        "minstrength",  tktMinstrength,
        "attack",       tktAttack,
        "item",         tktItem,
        "skill",        tktSkill,
        "nonstackable", tktNonStackable,
        "stackable",    tktStackable,
        "depletable",   tktDepletable,
        "coordinates",  tktCoordinates,
        "travel",       tktTravel,
        "world",        tktWorld,
        "generate",     tktGenerate,
        "type",         tktType,
        "hint",         tktHint,
        "object",       tktObject,
        
        "option",       tktOption,
        "option_token", tktOptionToken,
        "option_string",tktOptionString,
        "option_bool",  tktOptionBool,
        "option_int",   tktOptionInt,
        "option_float", tktOptionFloat,
        "false",        tktFalse,
        "true",         tktTrue,
        "like",         tktLike,
        
        "prototype",    tktPrototype,
        "equip_slot",   tktEquipSlot,
        "key",          tktKey,
        "dv",           tktDv,
        
        "damage",       tktDamage,
        "hp",           tktHp,
        "fp",           tktFp,
        "mp",           tktMp,
        "elemental",    tktElemental,
        "maximum",      tktMaximum,
        "multiplier",   tktMultiplier,
        
        "obstacle",     tktObstacle,
        "bulletproof",  tktBulletproof,
        "visionproof",  tktVisionproof,
        "height",       tktHeight,
        "mainmap",      tktMainmap,
        "underground",  tktUnderground,
        
        "strength",     tktStrength,
        "constitution", tktConstitution,
        "dexterity",    tktDexterity,
        "intelligence", tktIntelligence,
        "charisma",     tktCharisma,
        "willpower",    tktWillpower,
        "luck",         tktLuck,
        "perception",   tktPerception,
        "beauty",       tktBeauty,
        
        "helm",         ettHelm,
        "amulet",       ettAmulet,
        "hand",         ettHand,
        "offhand",      ettOffhand,
        "twohands",     ettTwoHands,
        "launcher",     ettLauncher,
        "ammunition",   ettAmmunition,
        "armor",        ettArmor,
        "clothes",      ettClothes,
        //"leftring",     ettLeftRing,
        //"rightring",    ettRightRing,
        "gloves",       ettGloves,
        "belt",         ettBelt,
        "boots",        ettBoots;
    //end of token registration
    
    {
        Parser parser("data/monsters");
        //using ../data/monsters
        while (parser.tryNextToken()) {
            int token = parser.lastToken();
            assert(token == tktMonster);
            MonsterClass monsterClass;
            int currentIndex = _monsterClasses.count();
            monsterClass.load(parser, currentIndex);
            _namesToMonsters[monsterClass.name] = currentIndex;
            _monsterClasses.append(monsterClass);
            //cout << "tables.cpp: data/monsters: currentIndex=" << currentIndex << endl;
        }
    }
    {
        Parser parser("data/items");
        while (parser.tryNextToken()) {
            int token = parser.lastToken();
            assert(token == tktItem);
            ItemClass itemClass;
            itemClass.load(parser);
            _namesToItems[itemClass.name] = _itemClasses.count();
            _itemClasses.append(itemClass);
//             itemClass.print();
        }
    }
    {
        Parser parser("data/background");
        while (parser.tryNextToken()) {
            int token = parser.lastToken();
            assert(token == tktTile);
            BackgroundClass backgroundClass;
            backgroundClass.load(parser);
            _namesToBackgrounds[backgroundClass.name] = _backgroundClasses.count();
            _backgroundClasses.append(backgroundClass);
        }
    }
    {
        Parser parser("data/object");
        while (parser.tryNextToken()) {
            int token = parser.lastToken();
            assert(token == tktObject);
            ObjectClass objectClass;
            objectClass.load(parser);
            _namesToObjects[objectClass.name] = _objectClasses.count();
            _objectClasses.append(objectClass);
        }
    }
}

Tables:: Tables() {
}

Tables:: ~Tables() {
    //cout << "deleting tables" << endl;
}

Tables& Tables:: getInstance() {
    if (!instance) {
        instance = new Tables();
        instance->init();
    }
    return *instance;
}

/*!
   \brief clears registered token list and adds token to list. must be 
          followed by call to operator ,(int), e.g. parser += "token", 5
 */
Tables& Tables::operator +=(QString c)
{
    _namesToTokens.clear();
    _tokensToNames.clear();
    waitingID = true;
    lastRegToken = c;
    
    return *this;
}


/*!
   \brief adds token to list. must be followed by call to operator ,(int), 
          e.g. parser += "token", 5;
 */
Tables& Tables::operator ,(QString c)
{
    assert(!waitingID); //integer id for previous token expected

    waitingID = true;
    lastRegToken = c;
    
    return *this;
}

/*!
   \brief sets integer id for previously specified token (operators += and ,)
          must be called after += or , with QString  parameter.
   \param number token id          
 */
Tables& Tables::operator ,(int number)
{
    assert(waitingID); //new token expected

    waitingID = false;
    assert(!_namesToTokens.contains(lastRegToken));
    _namesToTokens.insert(lastRegToken, number);
    _tokensToNames.insert(number, lastRegToken);
    return *this;
}

int Tables:: nameToItem(const QString name) {
    return getInstance()._namesToItems[name];
}

const ItemClass& Tables:: itemToClass(const int item) {
    return getInstance()._itemClasses[item];
}

int Tables:: itemsCount() {
    return getInstance()._itemClasses.count();
}

int Tables:: nameToMonster(const QString name) {
    return getInstance()._namesToMonsters[name];
}

MonsterClass& Tables:: monsterToClass(const int monster) {
    return getInstance()._monsterClasses[monster];
}

int Tables:: monstersCount() {
    return getInstance()._monsterClasses.count();
}

int Tables:: nameToBackground(const QString name) {
    return getInstance()._namesToBackgrounds[name];
}

BackgroundClass& Tables:: backgroundToClass(const int background) {
    return getInstance()._backgroundClasses[background];
}

int Tables:: backgroundsCount() {
    return getInstance()._backgroundClasses.count();
}

/** \brief Searches for a closest match background
 * 
 *  Flags must be the same, height must be closest
 * 
 *  \returns closest match, -1 on no match
 */
int Tables:: search(const BackgroundClass& criteria, int checkedFlags, bool debug) {
    int result = -1;
    int bestRange = INT_MAX;
    const QList<BackgroundClass>& classes = getInstance()._backgroundClasses;
    if (debug) {
        cout << "criteria" << endl;
        criteria.print();
    }
    if (classes.count()) {
        for (int i=0; i<classes.count(); i++) {
            if (debug) {
                classes[i].print();
            }
            if ((criteria.flags & checkedFlags) != (classes[i].flags & checkedFlags)) continue;
            if (!criteria.type.isEmpty()) {
                if (criteria.type != classes[i].type) {
                    continue;
                }
            }
            int range = abs(criteria.height - classes[i].height);
            if (range < bestRange) {
                result = i;
                bestRange = range;
            }
        }
    }
    return result;
}

int Tables:: nameToObject(const QString name) {
    return getInstance()._namesToObjects[name];
}

ObjectClass& Tables:: objectToClass(const int object){
    return getInstance()._objectClasses[object];
    
}

int Tables:: objectsCount() {
    return getInstance()._objectClasses.count();
}

int Tables:: searchObject(const QVariantMap& criteria) {
    QList<int> results;
    const QList<ObjectClass>& classes = getInstance()._objectClasses;
    if (classes.count()) {
        for (int i=0; i<classes.count(); i++) {
            const QVariantMap& map = classes[i].options;
            bool meetsCriteria = true;
            foreach(const QString optname, criteria.keys()) {
                //cout << optname.toStdString() << ", " << map[optname].toString().toStdString() << endl;
                if (criteria[optname] != map[optname]) {
                    meetsCriteria = false;
                    break;
                }
            }
            if (meetsCriteria) {
                //cout << "meets criteria " << i << endl;
                results.append(i);
            }
        }
    }
    if (results.count()) {
        return results[rand() % results.count()];
    }
    else {
        return -1;
    }
}

int Tables:: nameToToken(const QString name) {
    return getInstance()._namesToTokens[name];
}

QString Tables:: tokenToName(const int token) {
    return getInstance()._tokensToNames[token];
}

bool Tables:: tokenNameExists(QString name) {
    return getInstance()._namesToTokens.contains(name);
}

int Tables:: nameToSkill(const QString name) {
    QMap<QString,int>& namesToSkills = getInstance()._namesToSkills;
    QList<QString>& skillsToNames = getInstance()._skillsToNames;
    if (namesToSkills.contains(name)) {
        return namesToSkills[name];
    }
    else {
        int skill = skillsToNames.count();
        skillsToNames.append(name);
        namesToSkills[name] = skill;
        return skill;
    }
}

QString Tables:: skillToName(const int skill) {
    return getInstance()._skillsToNames[skill];
}

void Tables:: addKeyCode(QString code) {
    if (!getInstance()._keyCodes.contains(code)) {
        getInstance()._keyCodes.append(code);
    }
}

int Tables:: keyCodesCount() {
    return getInstance()._keyCodes.count();
}

QString Tables:: getRandomKeyCode() {
    int index = rand() % keyCodesCount();
    return getInstance()._keyCodes[index];
}
