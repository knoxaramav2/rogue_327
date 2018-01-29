#ifndef DUNGEON
#define DUNGEON

#define CELL(x,y) ((x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH))

typedef struct Dungeon{
    unsigned * screen;
} Dungeon;

#endif