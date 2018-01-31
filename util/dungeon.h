#ifndef DUNGEON
#define DUNGEON

#define CELL(x,y) ((x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH))

typedef struct Dungeon{
    unsigned * screen;

    int ** roomInfo;
    int roomCount;
} Dungeon;

Dungeon * createDungeon();
Dungeon * generateDungeon();

//Attributes
unsigned getHardness(unsigned cell);
unsigned setHardness(unsigned cell, unsigned hardness);
unsigned getImmutable(unsigned cell);
unsigned setImmutable(unsigned cell, unsigned immutable);
char getSymbol(unsigned cell);
char setSymbol(unsigned cell, char symbol);

#endif