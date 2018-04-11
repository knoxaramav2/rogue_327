#include <string.h>
//#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "dungeon.h"
#include "config.h"
#include "console.h"
#include "algo.h"
#include "defs.h"

//Config config;
//Console __console;

using namespace std;

WINDOW * createWindow(int height, int width, int yOffset, int xOffset){
    WINDOW * ret;

    ret = newwin(height, width, yOffset, xOffset);

    return ret;
}

Console::Console(){
    //initscr();
    screen = newterm(NULL, stdin, stdout);

    if (screen == NULL){
        return;
    }

    cbreak();
    noecho();
    keypad(stdscr, true);
    
    //curs_set(false);
    start_color();

    init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);


    infoBar = createWindow(1, DUNGEON_WIDTH, 0, 0);
    gameWindow = createWindow(DUNGEON_HEIGHT-1, DUNGEON_WIDTH - 1, 1, 0);
    menuWindow = createWindow(2, DUNGEON_WIDTH, DUNGEON_HEIGHT, 0);

    clear();
    mvwprintw(infoBar, 0, 0, "INFO |");
    refresh();
}

Console::~Console(){
    delscreen(screen);
}

void Console::update(){
    clear();
    wrefresh(infoBar);
    wrefresh(gameWindow);
    wrefresh(menuWindow);
    
    box(gameWindow, 0, 0);
}

void Console::clearGameWindow(){
    wclear(gameWindow);
}

void Console::displayMonsterStats(vector <string> stats, int yVal){

    clearGameWindow();

    if (yVal < 0)
        yVal = 0;
    if (yVal >= stats.size())
        yVal = stats.size()-1;

    for (int i = yVal; i < stats.size(); ++i){
        mvwprintw(gameWindow, i+1, 2, "%s", stats[i].c_str());
    }

    wrefresh(gameWindow);
    refresh();
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


void updatePlayerMap(Dungeon * d){

    unsigned * pMap = d->player->playerMap;
    unsigned * dMap = d->screen;
    Player * p = d->player;

    int minX = p->x >= 2  ? p->x - 2: 0;
    int minY = p->y >= 2  ? p->y - 2: 0;
    int maxX = p->x <= DUNGEON_WIDTH - 2 ? p->x + 2: DUNGEON_WIDTH;
    int maxY = p->y <= DUNGEON_HEIGHT - 2 ? p->y + 2: DUNGEON_HEIGHT - 1;

    for (int x = minX; x <= maxX; ++x){
        for (int y = minY; y <= maxY; ++y){
            pMap[CELL(x, y)] = dMap[CELL(x,y)];
        }
    }
}

void updateAll(){

}

//patch hole left by moving monster as long as it was previously visible
void updateMove(Dungeon * d, Entity * e){

    Entity * p = d->player;

    if (!isWithinBox(CELL(p->lastX, p->lastY), 2, 2, CELL(e->lastX, e->lastY)))
        return;

    mvwprintw(__console.gameWindow, e->lastY, e->lastX, "%c", getSymbol(d->screen[CELL(e->lastX, e->lastY)]));
}

void renderMap(unsigned * map){
    wclear(__console.gameWindow);

    for (int x = 1; x < DUNGEON_WIDTH; ++x){
        for (int y = 1; y < DUNGEON_HEIGHT; ++y){
            mvwprintw(__console.gameWindow, y, x, "%c", getSymbol(map[CELL(x, y)]));
        }
    }

    box(__console.gameWindow, 0, 0);
    refresh();
}

unsigned * getActiveMap(Dungeon * d){
    return d->fogOfWar ? d->player->playerMap : d->screen;
}

void updateScreen(Dungeon * d){

    //renderDistance(d, false);
    //__console.update();
    //return;

    Player * p = d->player;
    unsigned * screen = d->screen;
    unsigned * pcScreen = p->playerMap;
    unsigned * rScreen = NULL;

    updatePlayerMap(d);

    int minX, minY, maxX, maxY;

    if (d->fogOfWar){
        minX = p->x >= 2  ? p->x - 2: 0;
        minY = p->y >= 2  ? p->y - 2: 0;
        maxX = p->x <= DUNGEON_WIDTH - 2 ? p->x + 2: DUNGEON_WIDTH;
        maxY = p->y <= DUNGEON_HEIGHT - 2 ? p->y + 2: DUNGEON_HEIGHT - 1;
        rScreen = pcScreen;
    } else {
        minX = 2;
        minY = 2;
        maxX = DUNGEON_WIDTH - 2;
        maxY = DUNGEON_HEIGHT - 2;
        rScreen = screen;
    }

    //place surrounding map
    for (int x = minX; x <= maxX; ++x){
        for (int y = minY; y <= maxY; ++y){
            mvwprintw(__console.gameWindow, y, x, "%c", getSymbol(rScreen[CELL(x, y)]));
        }
    }

    //draw monsters
    for(int i = 0; i < config.numNpc; ++i){
        Entity * e = d->npcs[i];
        int pCell = CELL(p->x, p->y);

        //zone where player can see
        if (isWithinBox(pCell, 2, 2, CELL(e->x, e->y)) || !d->fogOfWar){
            int c = randIn(0, e->colors.size());
            wattron(__console.gameWindow,COLOR_PAIR(e->colors[c]));
            mvwprintw(__console.gameWindow, e->y, e->x, "%c", e->symbol);
            wattroff(__console.gameWindow,COLOR_PAIR(e->colors[c]));
        }        
    }

    //draw player
    wattron(__console.gameWindow,COLOR_PAIR(COLOR_GREEN));
    mvwprintw(__console.gameWindow, p->y, p->x, "@");
    wattroff(__console.gameWindow,COLOR_PAIR(COLOR_GREEN));

    __console.update();
}

void initColors(){
    init_pair(1, COLOR_RED, COLOR_BLACK);
}