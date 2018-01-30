#include "dungeon.h"
#include "defs.h"

//Attributes
unsigned setAttribute(unsigned cell, unsigned mask){
    //clear current attribute
    cell &= ~mask;
    return cell | mask;
}

unsigned getHardness(unsigned cell){
    return (cell & HARD_MASK) >> HARD_OFFSET;
}

unsigned setHardness(unsigned cell, unsigned hardness){
    return setAttribute(cell, (hardness << HARD_OFFSET) & HARD_MASK);
}

unsigned getImmutable(unsigned cell){
    return (cell & IMMUTABLE_MASK) >> MUTABLE_OFFSET;
}

unsigned setImmutable(unsigned cell, unsigned immutable){

    setAttribute(cell, IMMUTABLE_MASK * (immutable != 0));
}

char getSymbol(unsigned cell){
    return cell & SYMBOL_MASK;
}

char setSymbol(unsigned cell, char symbol){
    return symbol | (cell & !SYMBOL_MASK);
}