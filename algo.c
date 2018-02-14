#include <stdlib.h>
#include <string.h>

#include "algo.h"

#include "defs.h"


int getX(int i){
    return i % DUNGEON_WIDTH;
}

int getY(int i){
    return i / DUNGEON_HEIGHT;
}

//QUEUE
Queue * createQueue(){
    Queue * ret = malloc(sizeof(Queue));

    ret->size = 0;
    ret->head = 0;
    ret->tail = 0;

    return ret;
}

void swapLeft(Queue * q, QueueNode * n){
        QueueNode * l = n->prev;
        QueueNode * r = n->next;

        //Swap left
        if (l->prev){
            l->prev->next = n;
            n->prev = l->prev->next;
        } else {
            n->prev = 0;
            q->head = n;
        }

        if (r){
            r->prev = l;
            l->next = r;
        } else {
            l->next = 0;
            q->tail = l;
        }

        l->prev = n;
        n->next = l;
}

void bubbleNode(Queue * q, QueueNode * n){
    while(n->prev != 0 && n->prev->dist < n->dist){
        swapLeft(q, n);
    }
}

QueueNode * addToQueue(Queue * q, int id){

    QueueNode * n = malloc(sizeof(QueueNode));
    n->dist = 255;
    n->id = id;
    n->next = 0;
    n->prev = 0;

    if (q->head == 0){
        q->head = q->tail = n;
    } else {
        q->tail->next = n;
        n->prev = q->tail;
        q->tail = n;
    }

    return n;
}

int isEmpty(Queue * q){
    return q->size > 0;
}

int getAdjacentWeight(Dungeon * d, int x, int y, int i){
    int coord = CELL(x, y);

    int dx, dy;

    switch (i){
        case 0: dx = x-1; y-1; break;
        case 1: dx = x; y-1; break;
        case 2: dx = x+1; y-1; break;
        case 3: dx = x-1; y; break;
        case 4: dx = x+1; y; break;
        case 5: dx = x-1; y+1; break;
        case 6: dx = x; y+1; break;
        case 7: dx = x+1; y+1; break;
        default: return -1;
    }

    int c = CELL(dx, dy);
    if (c<0 || c>DUNGEON_HEIGHT * DUNGEON_WIDTH)
        return -1;

    return getHardness(d->screen[CELL(x, y)]);
}

int popQueue(Queue * q){

    q->head->next->prev = 0;
    QueueNode * h = q->head->next;
    free(q->head);
    q->head = h;
    q->head->prev = 0;

    return 0;
}

QueueNode * peekNode(Queue * q){
    return q->head;
}

//Pop root from heap, assign zero
//Update distance to all adjacent cells
//Find next adjacent cell to root with lowest value
//
void calcDistMap(Dungeon * d, int allowTunnel){

    int x = d->player->x;
    int y = d->player->y;

    const int D_SIZE = DUNGEON_HEIGHT * DUNGEON_WIDTH;

    if (d->_distanceMap != 0){
        free(d->_distanceMap);
    }

    d->_distanceMap = malloc(D_SIZE * sizeof(unsigned));
    memset(d->_distanceMap, 255, D_SIZE);
    d->_distanceMap[CELL(d->player->x, d->player->y)] = 0;

    Queue * q = createQueue();
    QueueNode * qArr [D_SIZE];

    //generate queue/populate index array
    int i = 0;
    for (; i < D_SIZE; ++i){
        qArr[i] = addToQueue(q, i);

        //push origin to front
        if (qArr[i]->id == CELL(x, y)){
            qArr[i]->dist = 0;
            bubbleNode(q, qArr[i]);
        }
    }

    while (!isEmpty(q)){

        QueueNode * u = peekNode(q);

        i = 0;
        for (; i < 8; ++i){

            int w = getAdjacentWeight(d getX(u->id), getY(u->id), i);

            /*if (){
                
            }*/
        }

        popQueue(q);

    }

    //generate heap

}

int randIn(int min, int max){
    return (rand()%(max-min)) + min;
}