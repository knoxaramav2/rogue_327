#ifndef GAME
#define GAME

#include "dungeon.h"

class TurnItem{

    public:

    Entity * e;
    int val;
    TurnItem * next;
};

void setupGameState(Dungeon * d);
void spawnPlayers(Dungeon * d);
void spawnItems(Dungeon * d);
void updateTurn(Dungeon ** d);
Entity * attack(Entity * attacker, Dungeon *d);
void moveByStrategy(Entity * e, Dungeon * d, int *toX, int *toY);

#endif