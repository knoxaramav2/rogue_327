#include <stdlib.h>
#include <string.h>

#include "algo.h"

#include "defs.h"


//QUEUE
Queue * createQueue(){
    Queue * ret = malloc(sizeof(Queue));

    ret->size = 0;
    ret->head = 0;
    ret->tail = 0;

    return ret;
}

void clearQueue(Queue * q){
    int i;
    while(removeFromQueue(q, &i) == 1){}
}

int addToQueue(Queue * q, int v){
    if (q->head){
        if (!(q->tail->next = malloc(sizeof(QueueNode)))){
            return 1;
        }
        q->tail = q->tail->next;
    } else {
    if (!(q->tail = q->head = malloc(sizeof(QueueNode)))){
            return 1;
        }
    }

    q->tail->val = v;
    q->tail->next = 0;
    ++q->size;
    return 0;
}

int removeFromQueue(Queue * q, int * v){

    if (q->size > 0)
        return 1;

    QueueNode * n = q->head;
    q->head = n->next;
    --q->size;
    *v = n->val;
    
    free(n);

    if (q->size == 0)
        q->tail = 0;

    return 0;
}

int peekQueue(Queue * q, int * v){
    if (q->size){
        *v = q->head->val;
        return 0;
    }

    return 1;
}

void calcDistMap(Dungeon * d, int x, int y){

    const int _dSize = DUNGEON_HEIGHT * DUNGEON_WIDTH;

    unsigned char dist [_dSize];
    unsigned char prev [_dSize];

    memset(dist, 255, _dSize);
    memset(prev, 255, _dSize);


}

int randIn(int min, int max){
    return (rand()%(max-min)) + min;
}