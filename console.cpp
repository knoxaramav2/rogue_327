#include <string.h>
//#include <stdio.h>
#include <ncurses.h>

#include "dungeon.h"
#include "config.h"
#include "console.h"
#include "algo.h"
#include "defs.h"

//Config config;
//Console __console;

WINDOW * createWindow(int height, int width, int yOffset, int xOffset){
    WINDOW * ret;

    //ret = newwin(height, width, yOffset, xOffset);
    ret = newwin(height, width, yOffset, xOffset);
    box(ret, 0, 0);

    return ret;
}

Console::Console(){
    //initscr();
    screen = newterm(NULL, stdin, stdout);

    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(false);
    start_color();

    init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);


    //gameWindow = createWindow(DUNGEON_HEIGHT, DUNGEON_WIDTH, 0, 0);
    gameWindow = createWindow(DUNGEON_HEIGHT, DUNGEON_WIDTH - 1, 0, 0);
    menuWindow = createWindow(5, DUNGEON_WIDTH, 0, DUNGEON_HEIGHT);

    move(0,0);
    clear();
    refresh();
}

Console::~Console(){
    delscreen(screen);
}

void Console::update(){
    clear();
    wrefresh(gameWindow);
    //wrefresh(menuWindow);
}


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

/*
int renderScreen(Dungeon * dungeon){

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){

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
}*/

int renderDistance(Dungeon * dungeon, int allowTunnel){

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){

        int x = getX(i);
        int y = getY(i);

        Entity * player = dungeon->player;
        char monster = hasMonster(dungeon, i);
        
        mvwprintw(__console.gameWindow, y, x, "/");

        if (monster){
            mvwprintw(__console.gameWindow, y, x, "M");
        } else if (CELL(player->x, player->y) == i){
            mvwprintw(__console.gameWindow, y, x, "P");
        } else {
            if (allowTunnel == 0 && getHardness(dungeon->screen[i]) > 1)
                mvwprintw(__console.gameWindow, y, x, "^");
            else
                mvwprintw(__console.gameWindow, y, x, "%d", getLastDigit(dungeon->_distanceMap[i]));
        }

        //if (i % DUNGEON_WIDTH == 0)
            //wrefresh(stdscr);
    }
}

void updateScreen(Dungeon * d){

    renderDistance(d, false);
    __console.update();
    return;

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
            //p->playerMap[qidx] = screen[idx];
        }
    }

    //draw monsters
    for(int i = 0; i < config.numNpc; ++i){
        Entity * e = d->npcs[i];
        int c = randIn(0, e->colors.size());
        attron(COLOR_PAIR(e->colors[c]));
        mvprintw(e->y, e->x, "%c", e->symbol);
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