#include <string.h>
#include <stdio.h>

#include "console.h"
#include "defs.h"

int printBoarder(){
    int i = 1;

    for(; i <= DUNGEON_WIDTH; ++i){
        
    }
}

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

    printBoarder();

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