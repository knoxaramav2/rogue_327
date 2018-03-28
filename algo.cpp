#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "algo.h"
#include "console.h"

#include "defs.h"

int getY(int i){
    return i / DUNGEON_WIDTH;
}

int getX(int i){
    return i - ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
}

/*
Direction codes

5 6 7
3   4
0 1 2
*/
int getAdjacentIndex(int x, int y, int i){

    int dx, dy;

    switch (i){
        case BOTTOM_LEFT: dx = x-1; dy = y-1; break;
        case BOTTOM_MIDDLE: dx = x; dy = y-1; break;
        case BOTTOM_RIGHT: dx = x+1; dy = y-1; break;
        case MIDDLE_LEFT: dx = x-1; dy = y; break;
        case MIDDLE_RIGHT: dx = x+1; dy = y; break;
        case TOP_LEFT: dx = x-1; dy = y+1; break;
        case TOP_MIDDLE: dx = x; dy = y+1; break;
        case TOP_RIGHT: dx = x+1; dy = y+1; break;
        default: return -1;
    }

    if (dx < 0 || dx >= DUNGEON_WIDTH || dy < 0 || dy >= DUNGEON_HEIGHT)
        return -1;

   return CELL(dx, dy);
}

bool isValidSpace(int x, int y){
    if (x <= 0 || x >= DUNGEON_WIDTH ||
        y <= 0 || y >= DUNGEON_HEIGHT)
    return false;
    
    return true;
}

bool isValidSpace(int pos){
    int x = getX(pos);
    int y = getY(pos);

    return isValidSpace(x, y);
}

//QUEUE
Queue * createQueue(){
    Queue * ret = (Queue *) malloc(sizeof(Queue));

    ret->size = 0;
    ret->head = 0;
    ret->tail = 0;

    return ret;
}

void swapLeft(Queue * q, QueueNode * n){

        if (n->prev == 0)
            return;

        QueueNode * l = n->prev;
        QueueNode * r = n->next;
        QueueNode * lp = l->prev;

        if (lp){
            lp->next = n;
            n->prev = lp;
        } else {
            n->prev = 0;
        }

        if (r){
            r->prev = l;
            l->next = r;
        } else {
            l->next = 0;
        }

        n->next = l;
        l->prev = n;
}

void bubbleNode(Queue * q, QueueNode * n){

    unsigned long test = (unsigned long) n->prev;//TODO FIX THIS
    if (test < 5000){
        return;
    }

    if (n->next == 0){
        q->tail = n->prev;
    }

    while(n->prev != 0 && n->prev->dist > n->dist){
        swapLeft(q, n);
        //printf("Swapped %d @ %d for %d @ %d\r\n", n->id, n->dist, n->next->id, n->next->dist);
    }

    if (n->prev == 0){
        q->head = n;
    }
}

QueueNode * addToQueue(Queue * q, int id){

    QueueNode * n = (QueueNode *) malloc(sizeof(QueueNode));
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

    ++q->size;

    return n;
}

int isEmpty(Queue * q){
    return q->size == 0;
}

int getAdjacentWeight(unsigned * grid, int x, int y, int i){

    int c = getAdjacentIndex(x, y, i);

    if (c<0) return -1;

    return getHardness(grid[c]);
}

int popQueue(Queue * q){

    if (q->size == 0)
        return 0;

    --q->size;
    
    QueueNode * h = q->head->next;

    if (h == 0){
        return 0;
    }

    h->prev = 0;
    //free(q->head);
    q->head = h;

    

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

    d->_distanceMap = (unsigned *) malloc(D_SIZE * sizeof(unsigned));
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

    QueueNode * u = peekNode(q);
    int tx=0;

    while (!isEmpty(q)){

        //examine neighbors
        i = 0;
        int lowestIdx = 0;
        int lowestVal = 255;
        int currDist = u->dist;
        for (; i < 8; ++i){

            int ax = getX(u->id);
            int ay = getY(u->id);
            int aIdx = getAdjacentIndex(ax, ay, i);
            QueueNode * qn = qArr[aIdx];

            if (qn == 0 || aIdx < 0) continue; //not available

            if (getSymbol(d->screen[aIdx]) == ROCK_SPACE){
                printf("%c", getSymbol(d->screen[aIdx]));
            }

            int adjWght = getSymbol(d->screen[aIdx]) == ROCK_SPACE ? getHardness(d->screen[aIdx]) : 1;
            int adjDist;
            if (qArr[aIdx]){
                adjDist = qArr[aIdx]->dist;
            } else {
                continue;
            }

             int newDist = adjWght + currDist;

            if(newDist < adjDist || adjDist == 255){
                d->_distanceMap[aIdx] = newDist;
                qn->dist = newDist; 

                if (newDist < lowestVal){
                    lowestVal = newDist;
                    lowestIdx = i;
                }

                bubbleNode(q, qArr[aIdx]);
            }
        }
        
        popQueue(q);
        qArr[u->id] = 0;
        u = peekNode(q);
    }

    //generate heap

}

int randIn(int min, int max){
    return (rand()%(max-min)) + min;
}