#include <string.h>
#include <stdio.h>
#include <ncurses.h>

#include "dungeon.h"
#include "config.h"
#include "console.h"
#include "algo.h"
#include "defs.h"

//Config config;

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
/*
int clearScreen(){
    //TODO use ascii escape method instead
    int i = 0;
    for (; i < 30; ++i){
        printf("\n");
    }

    printf("\r");
}*/

int renderScreen(Dungeon * dungeon){

    printBoarder();

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){
        if (i % DUNGEON_WIDTH == 0){
            printf("\r\n");
        }
        unsigned cell = (i%(DUNGEON_WIDTH)) + ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
        Player * player = dungeon->player;

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

void updateScreen(Dungeon * d){

    Player * p = d->player;
    unsigned * screen = d->screen;
    unsigned * pcScreen = p->playerMap;

    int minX = p->x >= 2  ? p->x - 2: 0;
    int minY = p->y >= 2  ? p->y - 2: 0;
    int maxX = p->x <= DUNGEON_WIDTH - 2 ? p->x + 2: DUNGEON_WIDTH;
    int maxY = p->y <= DUNGEON_HEIGHT - 2 ? p->y + 2: DUNGEON_HEIGHT - 1;

    //place surrounding map
    for (int x = minX; x <= maxX; ++x){
        for (int y = minY; y <= maxY; ++y){
            mvprintw(y, x, "%c", getSymbol(screen[(x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH)]));
            //int idx = CELL(x, y);
            //p->playerMap[idx] = screen[idx];
        }
    }

    //draw monsters
    for(int i = 0; i < config.numNpc; ++i){
        Entity * e = d->npcs[i];
        int c = randIn(0, e->colors.size());
        std::string s;
        s += e->symbol;
        attron(COLOR_PAIR(e->colors[c]));
        mvprintw(e->y, e->x, s.c_str());
        attroff(COLOR_PAIR(e->colors[c]));
    }

    //draw player
    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(p->y, p->x, "@");
    attroff(COLOR_PAIR(COLOR_GREEN));

    wrefresh(stdscr);
}

void initColors(){
    init_pair(1, COLOR_RED, COLOR_BLACK);
}