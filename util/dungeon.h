#ifndef DUNGEON
#define DUNGEON

#define CELL(x,y) ((x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH))

typedef struct Dungeon{
    unsigned * screen;
} Dungeon;

Dungeon * generateDungeon();

//Attributes
unsigned getHardness(unsigned cell);
unsigned setHardness(unsigned cell, unsigned hardness);
unsigned getMutable(unsigned cell);
unsigned setMutable(unsigned cell, unsigned mutable);
char getSymbol(unsigned cell);
char setSymbol(unsigned cell, char symbol);

#endif