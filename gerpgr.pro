#TEMPLATE = app
TARGET += 
DEPENDPATH += .
INCLUDEPATH += . 

# Input
HEADERS += \
    algorithms/graph.hpp \
    engine/QEquipmentDialog.hpp \
    engine/QItemsChecklist.hpp \
    engine/QPickupDialog.hpp \
    engine/QSelectOneItemDialog.hpp \
    engine/engine.hpp \
    engine/qdiscardablepixmap.hpp \
    engine/qstatsevent.hpp \
    game/abstractaction.hpp \
    game/actionrandom.hpp \
    game/actiontravel.hpp \
    game/attackdescriptor.hpp \
    game/backgroundclass.hpp \
    game/coordinateslist.hpp \
    game/door.hpp \
    game/engineinterface.hpp \
    game/exception.hpp \
    game/faction.hpp \
    game/factions.hpp \
    game/game.hpp \
    game/gamemap.hpp \
    game/item.hpp \
    game/itemclass.hpp \
    game/itemslist.hpp \
    game/leader.hpp \
    game/mapobject.hpp \
    game/maps.hpp \
    game/masks.hpp \
    game/monster.hpp \
    game/monsterclass.hpp \
    game/monsters.hpp \
    game/objectclass.hpp \
    game/player.hpp \
    game/ruin.hpp \
    game/skill.hpp \
    game/stair.hpp \
    game/tables.hpp \
    game/testable.hpp \
    game/trigger.hpp \
    game/triggerplayer.hpp \
    game/triggerquestion.hpp \
    game/village.hpp \
    game/world.hpp \
    game/worlds.hpp \
    gui/QGUIWidget.hpp \
    iterators/abstractiterator.hpp \
    iterators/lineiterator.hpp \
    iterators/walliterator.hpp \
    parser/parser.hpp
    
SOURCES += \
    algorithms/graph.cpp \
    engine/QEquipmentDialog.cpp \
    engine/QItemsChecklist.cpp \
    engine/QPickupDialog.cpp \
    engine/QSelectOneItemDialog.cpp \
    engine/engine.cpp \
    engine/qdiscardablepixmap.cpp \
    engine/qstatsevent.cpp \
    game/actionrandom.cpp \
    game/actiontravel.cpp \
    game/attackdescriptor.cpp \
    game/backgroundclass.cpp \
    game/coordinateslist.cpp \
    game/door.cpp \
    game/faction.cpp \
    game/factions.cpp \
    game/game.cpp \
    game/gamemap.cpp \
    game/item.cpp \
    game/itemclass.cpp \
    game/itemslist.cpp \
    game/leader.cpp \
    game/mapobject.cpp \
    game/maps.cpp \
    game/monster.cpp \
    game/monsterclass.cpp \
    game/monsters.cpp \
    game/objectclass.cpp \
    game/player.cpp \
    game/ruin.cpp \
    game/skill.cpp \
    game/stair.cpp \
    game/tables.cpp \
    game/trigger.cpp \
    game/triggerplayer.cpp \
    game/triggerquestion.cpp \
    game/village.cpp \
    game/world.cpp \
    game/worlds.cpp \
    gui/QGUIWidget.cpp \
    iterators/lineiterator.cpp \
    iterators/walliterator.cpp \
    main.cpp \
    parser/parser.cpp
    

MOC_DIR = build
OBJECTS_DIR = build

DESTDIR = build

#QT += opengl