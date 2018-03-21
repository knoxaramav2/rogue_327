#include <string.h>
#include <stdio.h>

#include "config.h"
#include "console.h"
#include "algo.h"
#include "defs.h"

Config config;

int getLastDigit(unsigned v){
    return v%10;
}

char hasMonster(Dungeon * d, int idx){
    int x = getX(idx);
    int y = getY(idx);

    int i = 0;
    for (; i < config.numNpc; ++i){
        if (x == d->npcs[i]->x && y == d->npcs[i]->y){
            return d->npcs[i]->symbol;
        }
    }

    return 0;
}

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
        unsigned cell = (i%(DUNGEON_WIDTH)) + ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
        Entity * player = dungeon->player;

        char monster = hasMonster(dungeon, i);

        if (monster){
            printf("\x1B[31m%c\x1B[0m", monster);
        } else if (CELL(player->x, player->y) == cell){
            printf("\x1B[32m@\x1B[0m");
        } else {
            printf("%c", dungeon->screen[cell]);
        }
    }

    //render actors


    //render text below
    printf("\r\n|\r\n|\r\n|\r\n");
    fflush(stdout);
}

int renderDistance(Dungeon * dungeon, int allowTunnel){

    printBoarder();

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){
        if (i % DUNGEON_WIDTH == 0){
            printf("\r\n");
        }
        unsigned cell = (i%(DUNGEON_WIDTH)) + ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
        Entity * player = dungeon->player;

        char monster = hasMonster(dungeon, i);
        
        if (monster){
            printf("\x1B[31m%c\x1B[0m", monster);
        } else if (CELL(player->x, player->y) == cell){
            printf("\x1B[32m@\x1B[0m");
        } else {
            if (allowTunnel == 0 && getHardness(dungeon->screen[cell]) > 1)
                printf(" ");
            else
                printf("%d", getLastDigit(dungeon->_distanceMap[cell]));
        }
    }

    //render actors


    //render text below
    printf("\n|\n|\n|\r\n");
    fflush(stdout);
}