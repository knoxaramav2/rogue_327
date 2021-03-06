#ifndef DUNGEON
#define DUNGEON

#include <vector>

#include "entity.h"

#define CELL(x,y) ((x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH))

#define ROOM_XPOS_I   0
#define ROOM_YPOS_I   1
#define ROOM_WIDTH_I  2
#define ROOM_HEIGHT_I 3

class Dungeon{

    public:

    Dungeon();
    ~Dungeon();

    unsigned * screen;
    unsigned * _distanceMap;

    int ** roomInfo;
    int roomCount;

    Player * player;
    Entity ** npcs;

    std::vector<Item> items;

    //debug states
    bool fogOfWar;
};

//Dungeon * createDungeon();
Dungeon * generateDungeon(Dungeon * d);

//Attributes
unsigned getHardness(unsigned cell);
unsigned setHardness(unsigned cell, unsigned hardness);
unsigned getImmutable(unsigned cell);
unsigned setImmutable(unsigned cell, unsigned immutable);
char getSymbol(unsigned cell);
char setSymbol(unsigned cell, char symbol);

#endif