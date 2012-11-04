#include "monster.hpp"
#include <math.h>
#include <fstream>
#include "tables.hpp"
#include "maps.hpp"
#include "monsters.hpp"
#include <assert.h>

const int directionsX[8] = {0, 1, 0, -1, 1, 1, -1, -1};
const int directionsY[8] = {-1, 0, 1, 0, -1, 1, 1, -1};

const int maxDirectionsCount = 4;

Monster:: Monster() {
    clear();
}

Monster::~Monster() {
    Monsters::remove(this);
}

void Monster:: clear() {
    items.clear();
    equipment.clear();
    skills.clear();
    x = 0;
    y = 0;
    monsterType = 0;
    energy = 0;
    target = 0;
    
    hpfrac = 0;
    fpfrac = 0;
    mpfrac = 0;
    //level.xp = 1;
    skills[Tables::nameToSkill("level")].xp = 1;
    loaded = false;
}

void Monster:: load(Parser &parser) {
    QString className;
    parser >> className;
    monsterType = Tables::nameToMonster(className);
    name = Tables::monsterToClass(monsterType).name;
    pLoad(parser);
}

void Monster:: save(ofstream& file) const {
    assert(loaded);
    file << "    monster \"" << name.toStdString();
    file << "\" " << x << " " << y << endl;
    pSave(file, "        ");
    file << "    end" << endl;
}

void Monster:: random(int monsterType) {
    clear();
    this->monsterType = monsterType;
    MonsterClass& monsterClass = Tables::monsterToClass(monsterType);
    name = monsterClass.name;
    for (SkillRanges::iterator i=monsterClass.skillRanges.begin(); i!=monsterClass.skillRanges.end(); i++) {
        float fraction = (rand() % 1024) / 1024.0;
        float xp = i.value().first + fraction * (i.value().second - i.value().first);
        skills[i.key()].xp = xp;
    }
    for (int i=0; i<9; i++) {
        int d = monsterClass.maxAttributes[i] - monsterClass.minAttributes[i];
        attributes[i] = monsterClass.minAttributes[i];
        if (d) {
            attributes[i] += rand() % (d+1);
        }
    }
    resetMaximums();
    hp = maxhp;
    fp = maxfp;
    mp = maxmp;
}

int Monster::getMonsterType() {
    return monsterType;
}

void Monster:: loadHealthState(Parser &parser) {
    parser >> maxhp >> hp >> maxfp >> fp >> maxmp >> mp;
}

/* from file 'sheet', formulaes */
void Monster:: resetMaximums() {
    maxhp = attributes[Constitution] + attributes[Strength] / 3 + skillLevel(Tables::nameToSkill("level"));
    maxfp = attributes[Constitution] + attributes[Willpower] / 3 + skillLevel(Tables::nameToSkill("level"));
    maxmp = attributes[Willpower] + attributes[Charisma] / 3 + skillLevel(Tables::nameToSkill("level"));
    if (hp > maxhp) hp = maxhp;
    if (fp > maxfp) fp = maxfp;
    if (mp > maxmp) mp = maxmp;
}

void Monster:: energyMove() {
    assert(loaded);
    energy -= 100;
}

void Monster:: naturallyRegenerate() {
    assert(loaded);
    if (!isAlive()) return;
    if (isConcious()) {
        energy += attributes[Dexterity];
    }
    bool changed = false;
    // regenerates energy, hp, fp, mp
    if (hp < maxhp) {
        hpfrac += attributes[Constitution];
        changed |= hpfrac >= 600;
        hp += hpfrac / 600;
        hpfrac %= 600;
    }
    if (hp == maxhp) hpfrac = 0;
    if (fp < maxfp) {
        fpfrac += attributes[Constitution];
        changed |= fpfrac >= 300;
        fp += fpfrac / 300;
        fpfrac %= 300;
    }
    if (fp == maxfp) fpfrac = 0;
    if (mp < maxmp) {
        mpfrac += attributes[Willpower];
        changed += mpfrac >= 150;
        mp += mpfrac / 150;
        mpfrac %= 150;
    }
    if (mp == maxmp) mpfrac = 0;
    if (changed) {
        repaint();
    }
}

bool Monster:: isAlive() {
    //assert(loaded);
    return hp > 0;
}

bool Monster:: isConcious() {
    assert(loaded);
    return fp > 0;
}

int Monster:: chance(double ratingDiff) {
    assert(loaded);
    float ch = atan(ratingDiff / 4) / M_PI + 0.5;
    ch *= 100;
    int c = (int) ch;
    if (c < 3) c = 3;
    if (c > 97) c = 97;
    return c;
}

int Monster:: rollDice(int multiplier, int maximum) {
    assert(loaded);
    int result = 0;
    for (int i=0; i<multiplier; i++) {
        result += rand() % maximum + 1;
    }
    return result;
}

void Monster:: meleeAttack(Monster& target) {
    assert(loaded);
    energyMove(); //TODO: currently primitive, "to be improved..."
    
    /* first of all, select weapon:
    it may be:
        two handed
        one handed
        one weapon for each hand
        unarmed
    */
    // select attack descriptor
    if (equipment.contains(ettTwoHands)) { //twohanded
        //cerr << "monster.cpp: twohanded: not implemented" << endl;
        const ItemClass& itemClass = Tables::itemToClass(equipment[ettTwoHands].type());
        AttackDescriptor descriptor = itemClass.twohanded;
        descriptor.skills << Tables::nameToSkill("melee");
        descriptor.skills << Tables::nameToSkill("twohands");
        descriptor.skills << Tables::nameToSkill(itemClass.name);
        weaponAttack(target, descriptor, descriptor.skills);
    }
    else if (equipment.contains(ettHand)) { //onehanded: 1 or 2
        const ItemClass& itemClass = Tables::itemToClass(equipment[ettHand].type());
        AttackDescriptor descriptor = itemClass.onehanded;
        descriptor.skills << Tables::nameToSkill("melee")
                << Tables::nameToSkill("onehand")
                << Tables::nameToSkill(itemClass.name);
        AttackDescriptor descriptor2;
        bool isAmbidexterous = false;
        if (equipment.contains(ettOffhand)) {
            const ItemClass& itemClass2 = Tables::itemToClass(equipment[ettOffhand].type());
            if (false/* is shield */) {
            }
            else {
                isAmbidexterous = true;
                int penalty = max(0, descriptor.minStrength - skillLevel(Tables::nameToSkill("ambidexterity")));
                cout << " penalty=" << penalty;
                descriptor.attack -= penalty;
                descriptor.skills << Tables::nameToSkill("ambidexterity");
                
                descriptor2 = itemClass2.onehanded;
                penalty = max(0, descriptor2.minStrength - skillLevel(Tables::nameToSkill("ambidexterity")));
                cout << " penalty=" << penalty;
                descriptor2.attack -= penalty + 2;
                descriptor2.skills << Tables::nameToSkill("melee")
                        << Tables::nameToSkill("onehand")
                        << Tables::nameToSkill(itemClass2.name)
                        << Tables::nameToSkill("ambidexterity");
            }
        }
        weaponAttack(target, descriptor, descriptor.skills);
        if (isAmbidexterous) {
            weaponAttack(target, descriptor2, descriptor2.skills);
//             int handItem = equipment[ettHand].type();
//             int offhandItem = equipment[ettOffhand].type();
//             cout << "handItem " << handItem << " offhandItem " << offhandItem << endl;
        }
    }
    else { //unarmed
        AttackDescriptor descriptor = unarmedAttackDescriptor();
        weaponAttack(target, descriptor, descriptor.skills);
    }
}

/** \brief Defensive value
 * 
 * Defensive value is used when calculating chance to hit this monster. Higher value means monster will be harder to hit. It depends on monster's Dexterity, combat skills and equipment
 * 
 * \todo dv from equipment
 * 
 * \returns defensive value
 */
int Monster:: dv() const {
    assert(loaded);
    int d = attributes[Dexterity] / 3;
    QList<int> s;
    if (equipment.contains(ettTwoHands)) { //twohanded
        //cerr << "monster.cpp: twohanded: not implemented" << endl;
        const ItemClass& itemClass = Tables::itemToClass(equipment[ettTwoHands].type());
        s << itemClass.twohanded.skills
                << Tables::nameToSkill("melee")
                << Tables::nameToSkill("twohands")
                << Tables::nameToSkill(itemClass.name);
    }
    else if (equipment.contains(ettHand)) { //onehanded: 1 or 2
        //cerr << "monster.cpp: onehanded: not implemented" << endl;        
        const ItemClass& itemClass = Tables::itemToClass(equipment[ettHand].type());
        s << itemClass.onehanded.skills
                << Tables::nameToSkill("melee")
                << Tables::nameToSkill("onehand")
                << Tables::nameToSkill(itemClass.name);
        if (equipment.contains(ettOffhand)) {
            const ItemClass& itemClass2 = Tables::itemToClass(equipment[ettOffhand].type());
            if (false/*is shield*/) {
                cerr << "monster.cpp: dv(): shield dv not implemented" << endl;
            }
            else {
                s << itemClass2.onehanded.skills
                        << Tables::nameToSkill(itemClass2.name)
                        << Tables::nameToSkill("ambidexterity");
            }
        }
    }
    else { //unarmed
        s << unarmedAttackDescriptor().skills;
    }
    int def = 0;
    for (int i=0; i<s.count(); i++) {
        def += skillLevel(s[i]);
    }
    d += def / s.count();
    return d;
}

void Monster:: weaponAttack(Monster& target, AttackDescriptor& attDesc, QList<int> attackSkills) {
    assert(loaded);
    int attack = (attributes[Dexterity] + attributes[Strength] - 20) / 10;
    //int defense = target.attributes[Dexterity] / 3;
    attack += attDesc.attack;
    int skillfullness = 0;
    //int defensiveness = 0;
    for (int i=0; i<attackSkills.count(); i++) {
        attack += skillLevel(attackSkills[i]);
        skillfullness += skillLevel(attackSkills[i]);
    }
    skillfullness /= attackSkills.count();
    int ch = chance(attack - target.dv());
    cout << " chance=" << ch << "% ";
    if ((rand() % 100) >= ch) {
        cout << name.toStdString() << " misses " << target.name.toStdString() << ". ";
        return;
    }
    int maxHpDamage = attDesc.hMaximum;
    int hpMultiplier = attDesc.hCount;
    int hpModifier = attDesc.hDamage;
    int maxFpDamage = attDesc.fMaximum;
    int fpMultiplier = attDesc.fCount;
    int fpModifier = attDesc.fDamage;
    int hpDamage = rollDice(hpMultiplier, maxHpDamage + skillfullness/3) 
            + hpModifier + (attributes[Strength]) / 10;
    int fpDamage = rollDice(fpMultiplier, maxFpDamage + skillfullness/3) 
            + fpModifier + (attributes[Strength]) / 10;
    // TODO: finish
    int hpResistance = target.attributes[Constitution] / 10 /*+ pv*/;
    int fpResistance = target.attributes[Constitution] / 5;
    hpDamage -= hpResistance;
    fpDamage -= fpResistance;
    hpDamage = max(hpDamage, 0);
    fpDamage = max(fpDamage, 0);
    cout << name.toStdString().c_str();
    if ((target.fp + target.attributes[Constitution]) < 0) {
        hpDamage += fpDamage;
        fpDamage = 0;
    }
    if (hpDamage) {
        if (fpDamage) {
             cout << " hits ";
        }
        else {
            cout << " pierces ";
        }
    }
    else {
        if (fpDamage) {
            cout << " pushes ";
        }
        else {
            cout << " barely touches ";
        }
    }
    cout << target.name.toStdString() << ". ";
    float xp = max(0, min(target.hp, hpDamage)) + max(0, min(target.fp, fpDamage));
    target.hp -= hpDamage;
    target.fp -= fpDamage;
    xp *= 2500.0 / (ch * ch);
    for (int i=0; i<attackSkills.count(); i++) {
        if (increaseSkill(attackSkills[i], xp)) {
            resetMaximums();
        }
    }
    if (increaseSkill(Tables::nameToSkill("level"), xp)) {
        resetMaximums();
    }
    target.damaged();
}

/** \brief Tests whether monster is capable of equipping item into slot
 * 
 * Tests whether monster has enough stats, skills etc. to equip specified item into slot
 * 
 * \param itemID index of item to be equipped in Monster::items
 * 
 * \param equipSlot equipment slot to be equipped to
 * 
 * \returns whether monster is capable of equipping item into slot
 */
bool Monster:: canEquipAs(int itemID, int equipSlot) const {
    assert(loaded);
    //if (!items[itemID].canEquipAs(equipSlot)) {
    //    return false;
    //}
    const ItemClass& itemClass = Tables::itemToClass(items[itemID].type());
    // checking strength requirements
    switch (equipSlot) {
        case ettHand: {
            if (attributes[Strength] < itemClass.onehanded.minStrength) {
                return false;
            }
            break;
        }
        case ettOffhand: {
            // TODO: improvement for shields
            if (attributes[Strength] < itemClass.onehanded.minStrength) {
                return false;
            }
            break;
        }
        case ettTwoHands: {
            if (attributes[Strength] < itemClass.twohanded.minStrength) {
                return false;
            }
            break;
        }
        case ettAmmunition: {
            //TODO: launcher may be present, too
            if (attributes[Strength] < itemClass.thrown.minStrength) {
                return false;
            }
            break;
        }
        default: {
            if (itemClass.getEquipSlot() != equipSlot) {
                return false;
            }
        }
    }
    return true;
}

/** \brief Equips monster with item
 * 
 *  Equips monster with item whose index in inventory is \c itemID into slot \c equipSlot
 * 
 *  \param itemID index used to look for item in Monster::items. if \c itemID is -1, slot is unequipped
 * 
 *  \param equipSlot equip slot to be equipped or unequipped
 */
void Monster:: equip(int itemID, int equipSlot) {
    assert(loaded);
    if (itemID == -1) {
        if (equipment.contains(equipSlot)) {
            Item item = equipment.take(equipSlot);
            items.append(item);
            // TODO: it takes time to unequip
            cout << "unequipped" << endl;
        }
    }
    else if (canEquipAs(itemID, equipSlot)) {
        Item item;
        if (equipSlot == ettAmmunition) {
            item = items.takeAllAt(itemID);
        }
        else {
            item = items.takeOneAt(itemID);
        }
        if (equipment.contains(equipSlot)) {
            equip(-1, equipSlot);
        }
        equipment[equipSlot] = item;
        // TODO: it takes time to unequip
        cout << "equipped" << endl;
    }
}

/** \brief Tests whether equip slot can be equipped
 * 
 *  If, for example, monster is carrying something with both hands, then hand and offhand slots can't be used.
 * 
 * \todo restriction to equip rings when gloves are equipped 
 * 
 * \param equipSlot slot to be equipped
 * 
 * \returns a pair of \c boolean value and a \c QString message. \c boolean value will be \b true, if slot is equippable, else it will be \b false and message will contain human-readable \c QString with reason
 */
QPair<bool,QString> Monster:: isSlotEquippable(int equipSlot) {
    assert(loaded);
    switch (equipSlot) {
        case ettHand:
        case ettOffhand: {
            if (equipment.contains(ettTwoHands)) {
                return QPair<bool,QString>(false, "first unequip twohands slot");
            }
            break;
        }
        case ettTwoHands: {
            if (equipment.contains(ettHand) || equipment.contains(ettOffhand)) {
                return QPair<bool,QString>(false, "first unequip hand and offhand slots");
            }
            break;
        }
    }
    return QPair<bool,QString>(true, "");
}

/** \brief Constructs and returns attack descriptor for unarmed fighting for this monster
 */
AttackDescriptor Monster:: unarmedAttackDescriptor() const{
    assert(loaded);
    AttackDescriptor descriptor;
    descriptor.attack = 0;
    descriptor.hCount = 1;
    descriptor.hCount = 2;
    descriptor.hMaximum = 3;
    descriptor.fMaximum = 3;
    descriptor.hDamage = attributes[Strength] / 10;
    descriptor.fDamage = attributes[Strength] / 5;
    descriptor.skills.append(Tables::nameToSkill("unarmed"));
    descriptor.skills.append(Tables::nameToSkill("melee"));
    return descriptor;
}

int Monster:: squareRangeTo(Monster & target) {
    assert(loaded);
    int dx = x - target.x;
    int dy = y - target.y;
    return dx * dx + dy * dy;
}

int Monster:: squareRangeTo(int x, int y) {
    assert(loaded);
    int dx = this->x - x;
    int dy = this->y - y;
    return dx * dx + dy * dy;
}

// to avoid duplicates, this can only be called once, before deleting monster object
ItemsList Monster:: takeAllItems() {
    assert(loaded);
    //unequip all:
    items += equipment.values();
    equipment.clear();
    // TODO: also add monster's corpse
    return items;
}

/** \brief Prints monster name, level and attributes
 */
void Monster:: print() {
    assert(loaded);
    cout << "Monster " << name.toStdString().c_str() << endl;
    cout << "\tlevel "<< skillLevel(Tables::nameToSkill("level")) << endl;
    for (int i = 0; i < 9; i++) {
        cout << "\t" <<attributes[i] << endl;
    }
}

bool Monster:: increaseSkill(int skill, float value) {
    assert(loaded);
    return skills[skill].increase(value);
}

/** \brief Returns level of a skill
 */
int Monster:: skillLevel(int skill) const {
    //assert(loaded);
    if (skills.contains(skill)) {
        return skills[skill]();
    }
    else {
        return 0;
    }
}

bool Monster:: isMonster() {
    return true;
}

QString Monster:: getMapName() const {
    return mapName;
}

void Monster:: setMapName(const QString mapName) {
    assert(!loaded);
    this->mapName = mapName;
}

//bits of improved architecture:
/** \brief Moves monster from it's current map to a map named <code>mapName</code> at coordinates (x, y).
 * 
 * Doesn't check whether it can be moved here.
 * \returns false, if such map or square does not exist
 * 
 */
bool Monster:: teleport(const QString& mapName, int x, int y) {
    assert(loaded);
    int oldX = this->x;
    int oldY = this->y;
    GameMap* map = Maps::nameToMap(this->mapName);
    GameMap* newMap = Maps::nameToMap(mapName);
    if (!newMap) return false;
    //MapSquare* square = newMap->squareAt(x, y);
    //if (!square) return false;
    //if (square->canAcceptMonster(*this)) {
    if (newMap->canAcceptMonsterAt(x, y, *this)) {
        if (map == newMap) {
            this->x = x;
            this->y = y;
            map->monsterMoved(*this, oldX, oldY);
        }
        else {
            map->removeMonster(this);
            this->x = x;
            this->y = y;
            this->mapName = mapName;
            newMap->insertMonster(this);
            newMap->squareChange(x, y);
            emit mapChanged();
        }
        return true;
    }
    else {
        return false;
    }
}

void Monster:: moveBy(int dx, int dy) {
    assert(loaded);
    int oldX = x;
    int oldY = y;
    x += dx;
    y += dy;
    energyMove();
    Maps::nameToMap(this->mapName)->monsterMoved(*this, oldX, oldY);
}

void Monster:: aiMove() {
    assert(loaded);
    int oldX = x;
    int oldY = y;
    GameMap* map = Maps::nameToMap(mapName);
    assert(map != 0);
    // dumbly searches for target: selects player
    //cout << "inside aiMove()" << endl;
    if (!map->containsMonster(target)) {
        //target = &player;
        target = 0;
        //cout << "has no target" << endl;
        foreach (Monster* monster, map->monsters) {
            if (!monster->isMonster()) {
                target = monster;
            }
        }
    }
    if (target) {
        // seeks it's target
        int dx = x - target->x;
        int dy = y - target->y;
        int range = dx * dx + dy * dy;
        // try to melee-attack, if possible
        if (range == 1) {
            meleeAttack(*target);
            if (!target->isAlive()) {
                //cout << "Monster's target died" << endl;
                if (!target->isMonster()) return;
            }
        }
        else { //else try to move
            int direction = -1;
            for (int i=0; i<maxDirectionsCount; i++) {
                int newX = x + directionsX[i];
                int newY = y + directionsY[i];
                //MapSquare* square = map->squareAt(newX, newY);
                //if (!square) continue;
                //if (!square->canAcceptMonster(*this)) continue;
                if (!map->canAcceptMonsterAt(newX, newY, *this)) continue;
                int dx = target->x - newX;
                int dy = target->y - newY;
                int r = dx * dx + dy * dy;
                if (r < range) {
                    direction = i;
                    range = r;
                }
            }
            if (direction == -1) {
                //cout << "no direction found. sleeping\n";
                energyMove();
            }
            else {
                // simply move
                energyMove();
                x += directionsX[direction];
                y += directionsY[direction];
                map->monsterMoved(*this, oldX, oldY);
            }
        }
    }
    else { // no targets found. sleeping
        energyMove();
    }
}

void Monster:: damaged() {
    assert(loaded);
    repaint();
}

int Monster:: getX() const {
    return x;
}

int Monster:: getY() const {
    return y;
}

void Monster:: setX(int x) {
    assert(!loaded);
    this->x = x;
}

void Monster:: setY(int y) {
    assert(!loaded);
    this->y = y;
}

int Monster:: getHp() const {
    return hp;
}

int Monster:: getFp() const {
    return fp;
}

int Monster:: getMp() const {
    return mp;
}

int Monster:: getMaxHp() const {
    return maxhp;
}

int Monster:: getMaxFp() const {
    return maxfp;
}

int Monster:: getMaxMp() const {
    return maxmp;
}

void Monster:: finishedLoading() {
    loaded = true;
}

bool Monster:: hasKey(QString code) const {
    if (!items.empty()) {
        for (int i=0; i<items.count(); i++) {
            const ItemClass& itemClass = Tables::itemToClass(items[i].type());
            if (itemClass.stringFeatures.contains(tktKey)) {
                if (itemClass.stringFeatures[tktKey] == code) {
                    return true;
                }
            }
        }
    }
    foreach (const Item& item, equipment) {
        const ItemClass& itemClass = Tables::itemToClass(item.type());
        if (itemClass.stringFeatures.contains(tktKey)) {
            if (itemClass.stringFeatures[tktKey] == code) {
                return true;
            }
        }
    }
    return false;
}

void Monster:: pSave(ofstream& file, const char* paddingSpaces) const {
    assert(loaded);
    //attributes
    file << paddingSpaces;
    file << "attributes";
    for (int i=0; i<9; i++) {
        file << " " << attributes[i];
    }
    file << endl;
    //health
    file << paddingSpaces;
    file << "health " << hp << " " << fp << " " << mp << endl;
    //skills
    QList<int> skillIndexes = skills.keys();
    for (int i=0; i<skillIndexes.count(); i++) {
        file << paddingSpaces << "skill \"";
        file << Tables::skillToName(skillIndexes[i]).toStdString();
        file << "\" " << skills[skillIndexes[i]].xp << endl;
    }
    //items
    if (items.count()) {
        for (int i=0; i<items.count(); i++) {
            file << paddingSpaces;
            file << "item " << items[i].count() << " \"";
            file << items[i].name().toStdString() << "\"" << endl;
        }
    }
    //equipment
    for (int i=0; i<equipmentTypesCount; i++) {
        int index = i+equipmentTypeMask;
        if (equipment.contains(index)) {
            file << paddingSpaces;
            file << Tables::tokenToName(index).toStdString() << " " 
                    << equipment[index].count() << " \""
                    << Tables::itemToClass(equipment[index].type()).name.toStdString()
                    << "\"" << endl;
        }
    }
}

void Monster:: pLoad(Parser& parser) {
    parser >> x >> y;
    TokenType token;
    bool healthLoaded = false;
    do {
        token = (TokenType) parser.nextToken();
        switch (token) {
            case tktAttributes: {
                for (int i = 0; i < 9; i++) {
                    parser >> attributes[i];
                }
                resetMaximums();
                break;
            }
            case tktHealth: {
                parser >> hp >> fp >> mp;
                healthLoaded = true;
                break;
            }
            case tktSkill: {
                QString skillName;
                float xp;
                parser >> skillName >> xp;
                int skill = Tables::nameToSkill(skillName);
                skills[skill].xp = xp;
                break;
            }
            case tktItem: {
                QString itemName;
                int count;
                parser >> count >> itemName;
                Item item(Tables::nameToItem(itemName), count);
                items.append(item);
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                if ((token & getTokenTypeMask) == equipmentTypeMask) {
                    QString itemName;
                    int count;
                    parser >> count >> itemName;
                    Item item(Tables::nameToItem(itemName), count);
                    equipment[token] = item;
                }
                else {
                    throw EException(QString("monster.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
                }
            }
        }
    } while (token != tktEnd);
    //post-load calculations
    if (!healthLoaded) {
        hp = maxhp;
        fp = maxfp;
        mp = maxmp;
    }
}

void Monster:: repaint() {
    assert(loaded);
    GameMap* map = Maps::nameToMap(mapName);
    map->squareChange(x, y);
}
