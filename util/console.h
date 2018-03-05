#ifndef CONSOLE
#define CONSOLE

#include "dungeon.h"

int clearScreen();
int renderScreen(Dungeon * dungeon);
int renderDistance(Dungeon * dungeon, int);
int getLastDigit(unsigned);

#endif