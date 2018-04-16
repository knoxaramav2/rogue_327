#ifndef CONSOLE
#define CONSOLE

#include "ncurses.h"
#include <vector>

#include "dungeon.h"

class Console{

    SCREEN * screen;

    public:

    WINDOW * infoBar;
    WINDOW * gameWindow;
    WINDOW * menuWindow;

    Console();
    ~Console();

    void update();
    void clearGameWindow();
    void displayMonsterStats(std::vector <std::string>, int yVal);
    void printInfo(std::string);
};

extern Console __console;

//int clearScreen();
//int renderScreen(Dungeon * dungeon);
//int renderDistance(Dungeon * dungeon, int);
int getLastDigit(unsigned);
void updateScreen(Dungeon * d);
void initColors();
void updateMove(Dungeon * d, Entity * e);
void renderMap(unsigned * map);
unsigned * getActiveMap(Dungeon * d);

#endif