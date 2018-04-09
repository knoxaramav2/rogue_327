#ifndef ALGO
#define ALGO

#include "dungeon.h"

class QueueNode{

    public:
    int dist;
    int id;
    struct QueueNode * next, * prev;
};

class Queue{
    public:

    int size;
    QueueNode * head, * tail;
};

bool isMonsterVisible(Dungeon * d, Entity * m);
void calcDistMap(Dungeon * d, int allowTunnel);
int randIn(int min, int max);
int getY(int i);
int getX(int i);
int getIndex(int x, int y);
int getAdjacentIndex(int x, int y, int i);
bool isValidSpace(int pos);
bool isValidSpace(int x, int y);

#endif