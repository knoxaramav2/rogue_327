#ifndef GAME
#define GAME

#include "dungeon.h"

typedef struct TurnItem{
    Entity * e;
    int val;
    struct TurnItem * next;
}TurnItem;

void setupGameState(Dungeon * d);
void spawnPlayers(Dungeon * d);
void updateTurn(Dungeon * d);

#endif