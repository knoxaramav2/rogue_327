#ifndef ALGO
#define ALGO

#include "dungeon.h"

typedef struct QueueNode{
    int dist;
    int id;
    struct QueueNode * next, * prev;
} QueueNode;

typedef struct Queue{
    int size;
    QueueNode * head, * tail;
} Queue;

void calcDistMap(Dungeon * d, int allowTunnel);
int randIn(int min, int max);
int getY(int i);
int getX(int i);
int getAdjacentIndex(int x, int y, int i);

#endif