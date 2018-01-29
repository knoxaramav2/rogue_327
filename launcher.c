#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "generators.h"

//UTIL

int renderScreen(Dungeon *);

//Renderers
int clearScreen(){
    //TODO use ascii escape method instead
    int i = 0;
    for (; i < 30; ++i){
        printf("\n");
    }

    printf("\r");
}

int renderScreen(Dungeon * dungeon){
    //TODO stop clearing each update
    clearScreen();

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){
        if (i % DUNGEON_WIDTH == 0){
            printf("\r\n");
        }
        int cell = (i%(DUNGEON_WIDTH)) + ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
        printf("%c", (char) dungeon->screen[i]);
    }

    //render text below
    printf("\n|\n|\n|\r\n");
}

//Attributes
int getHardness(int cell){
    return cell & HARD_MASK;
}

int setHardness(int cell, int hardness){
    return ((cell & SYMBOL_MASK) | (cell & MUTABLE_MASK) & (cell & !HARD_MASK)) | hardness;
}

int getMutable(int cell){

}

int setMutable(int cell, int mutable){

}

char getSymbol(int cell){

}

char setSymbol(int cell, char symbol){
    
}

int main(int argc, char ** argv){

    srand(time(0));

    Dungeon * dungeon = generateDungeon();

    //int i = 0;
    //for (; i < 20; ++i) dungeon->screen[CELL(i,i)] = '#';

    renderScreen(dungeon);

    return 0;
}