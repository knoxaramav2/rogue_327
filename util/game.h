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
Entity * attack(Entity * attacker, Dungeon *d);
void moveByStrategy(Entity * e, Dungeon * d, int *toX, int *toY);

#endif