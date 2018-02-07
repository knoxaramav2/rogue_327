#ifndef ALGO
#define ALGO

#include "dungeon.h"

typedef struct QueueNode{
    int val;
    struct QueueNode * next;
} QueueNode;

typedef struct Queue{
    int size;
    QueueNode * head, * tail;
} Queue;

Queue * createQueue();
void clearQueue(Queue * q);
int addToQueue(Queue * q, int val);
int removeFromQueue(Queue * q, int * val);
int peekQueue(Queue * q, int * val);

void calcDistMap(Dungeon * d, int x, int y);
int randIn(int min, int max);

#endif