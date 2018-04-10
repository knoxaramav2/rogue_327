#ifndef CONSOLE
#define CONSOLE

#include "ncurses.h"

#include "dungeon.h"

class Console{

    SCREEN * screen;

    public:

    WINDOW * gameWindow;
    WINDOW * menuWindow;

    Console();
    ~Console();

    void update();
};

extern Console __console;

//int clearScreen();
//int renderScreen(Dungeon * dungeon);
//int renderDistance(Dungeon * dungeon, int);
int getLastDigit(unsigned);
void updateScreen(Dungeon * d);
void initColors();

#endif