#ifndef MASKS_HPP_
#define MASKS_HPP_

/* Groups of tiles (examples):
          0. background (grass, water, forest)
          1. map object (village, road, tree, cave, wall, door)
          2. container (chest, )
          3. item (sword, armor, gold, ammunition, bazooka)
          4. monster (hero, dog, dragon, peasant)
          5. missile (thrown rock, grenade, shot arrow, bolt, missile)
 */

//const int backgroundTypeMask = 0x00000;
//const int mapObjectTypeMask  = 0x10000;
//const int containerTypeMask  = 0x20000;
//const int itemTypeMask       = 0x30000;
//const int monsterTypeMask    = 0x40000;
//const int missileTypeMask    = 0x50000;
const int tokenTypeMask      = 0x60000;
//const int skillTypeMask      = 0x70000;
const int equipmentTypeMask  = 0x80000;
const int getTokenTypeMask = 0xffff0000;

const int tokenMask          = 0x0FFFF;

enum TokenType {
    tktMap = tokenTypeMask,
    tktTile,
    tktTilesRange,
    tktPlayer,
    tktMonster,
    tktMonsters,
    tktAt,
    tktRandom,
    tktIn,
    tktAttributes,
    tktHealth,
    tktName,
    tktEnd,
    tktTrigger,
    tktQuestion,
    tktOnehanded,
    tktTwohanded,
    tktThrown,
    tktMinstrength,
    tktAttack,
    tktItem,
    tktSkill,
    tktNonStackable,
    tktStackable,
    tktDepletable,
    tktCoordinates,
    tktTravel,
    tktWorld,
    tktGenerate,
    tktType,
    tktHint,
    tktObject,
    
    tktOption,
    tktOptionToken,
    tktOptionString,
    tktOptionBool,
    tktOptionInt,
    tktOptionFloat,
    tktFalse,
    tktTrue,
    tktLike,
    
    tktPrototype,
    tktEquipSlot,
    tktKey,
    tktDv,
    
    tktDamage,
    tktHp,
    tktFp,
    tktMp, // for future
    tktElemental, // for future
    tktMaximum,
    tktMultiplier,
    
    tktObstacle,
    tktBulletproof,
    tktVisionproof,
    tktHeight,
    tktMainmap,
    tktUnderground,
    
    tktStrength,
    tktConstitution,
    tktDexterity,
    tktIntelligence,
    tktCharisma,
    tktWillpower,
    tktLuck,
    tktPerception,
    tktBeauty,
};


enum EquipmentType {
    ettHelm = equipmentTypeMask,
    ettAmulet,
    ettHand,
    ettOffhand,
    ettTwoHands,
    ettLauncher,
    ettAmmunition,
    ettArmor,
    ettClothes,
    //ettLeftRing,
    //ettRightRing,
    ettGloves,
    ettBelt,
    ettBoots,
    
    ettEquipmentTypesCount,
};

const int equipmentTypesCount = (int)ettEquipmentTypesCount & tokenMask;

#endif
