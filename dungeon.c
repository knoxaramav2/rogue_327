#include <stdlib.h>
#include <string.h>

#include "dungeon.h"
#include "defs.h"
#include "config.h"

Config config;

Dungeon * createDungeon(){

    Dungeon * dungeon  = malloc(sizeof(Dungeon));
    dungeon->screen = malloc((DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));
    dungeon->_distanceMap = malloc((DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));
    memset(dungeon->screen, setHardness(ROCK_SPACE, 50), (DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));//generate empty dungeon
    dungeon->roomCount = 0;
    dungeon->roomInfo = 0;
    dungeon->player = 0;
    dungeon->npcs = malloc(config.numNpc * sizeof(Entity*));

    return dungeon;
}

//Attributes
unsigned setAttribute(unsigned cell, unsigned value, unsigned mask){
    //clear current attribute
    cell &= ~mask;
    return cell | value;
}

unsigned getHardness(unsigned cell){
    return (cell & HARD_MASK) >> HARD_OFFSET;
}

unsigned setHardness(unsigned cell, unsigned hardness){
    return setAttribute(cell, (hardness << HARD_OFFSET) & HARD_MASK, HARD_MASK);
}

unsigned getImmutable(unsigned cell){
    return (cell & IMMUTABLE_MASK) >> MUTABLE_OFFSET;
}

unsigned setImmutable(unsigned cell, unsigned immutable){

    setAttribute(cell, IMMUTABLE_MASK * (immutable != 0), IMMUTABLE_MASK);
}

char getSymbol(unsigned cell){
    return cell & SYMBOL_MASK;
}

char setSymbol(unsigned cell, char symbol){
    return symbol | (cell & !SYMBOL_MASK);
}