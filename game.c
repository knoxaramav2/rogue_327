#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include "algo.h"
#include "game.h"
#include "config.h"
#include "console.h"
#include "defs.h"

Config config;
TurnItem * queue = 0;
int turn = 0;



void allowMove(Entity * e, Dungeon * d){
    if (e == d->player){

        printf(">> ");

        char c = getchar();

        switch(c){
            case 'w': --e->y; break;
            case 'a': --e->x; break;
            case 's': ++e->y; break;
            case 'd': ++e->x; break;
        }
    } else {
        
        int i = 0;
        int smallIdx = 0;
        unsigned smallVal = 999;
        for (;i < 8; ++i){
            unsigned w = d->_distanceMap[getAdjacentIndex(e->x, e->y, i)];
            if (w < smallVal && w >= 0){
                smallVal = w;
                smallIdx = i;
            }
        }

        //lose game
        if (smallVal == 0){
            config._run = 0;
            return;
        }

        e->x = getX(smallIdx);
        e->y = getY(smallIdx);

    }
}

void printQueue(){
    TurnItem * ti = queue;
    while(ti){
        printf("[%c %d] ", ti->e->symbol, ti->val);
        ti = ti->next;
    }

    printf("\r\n");
    fflush(stdout);
}

void queueTurn(Entity * e, int val){
    TurnItem * ti = malloc(sizeof(TurnItem));
    ti->e = e;
    ti->next = 0;
    ti->val = val;

    if (queue == 0){
        queue = ti;
    } else {
        ti->next = queue;
        TurnItem * prev = 0;

        while(ti->next && val >= ti->next->val){
            TurnItem * next = ti->next;
            ti->next = next->next;
            next->next = ti;

            if (prev){
                prev->next = next;
            }

            prev = next;
        }

        if (ti->next == queue){
            queue = ti;
        }
    }

    printQueue();
}

TurnItem * pop(){
    if (queue == 0)
        return 0;

    TurnItem * ti = queue;
    queue = queue->next;
    return ti;
}

TurnItem * peekQueue(){
    return queue;
}

void setupGameState(Dungeon * d){
    
}

void spawnPlayers(Dungeon * d){
    //place player in room
    int rId = randIn(0, d->roomCount);
    int * roomSet = d->roomInfo[rId];
    int xLoc = roomSet[0] + (roomSet[2] / 2 );
    int yLoc = roomSet[1] + (roomSet[3] / 2);

    d->player = createEntity('@', xLoc, yLoc, 0);
    queueTurn(d->player, 100);

    //randomly place monster
    int i;
    for (i=0; i<config.numNpc; ++i){
        int mx=-1, my;
        do{
            mx = randIn(0, DUNGEON_WIDTH);
            my = randIn(0, DUNGEON_HEIGHT);

            unsigned open = getSymbol(d->screen[CELL(mx, my)]);
            if (open == ROCK_SPACE)
                mx = -1;
        } while (mx < 0);

        //determine npcs type
        char sym = randIn(0, 4);
        switch(sym){
            case 0: sym='p'; break;
            case 1: sym='P'; break;
            case 2: sym='d'; break;
            case 3: sym='s'; break;
        }

        printf("%c at %d %d\r\n", sym, mx, my);

        d->npcs[i] = createEntity(sym, mx, my, 1);
        queueTurn(d->npcs[i], 1000/d->npcs[i]->speed);
    }
}

void updateTurn(Dungeon * d){
    ++turn;

    if (turn >= queue->val){
        Entity * e = queue->e;
        int val = queue->val;

        printf("Turn %d\r\n", turn);
        allowMove(e, d);
        renderScreen(d);
        pop();
        queueTurn(e, val + (1000/e->speed));

        //sleep(1);
    }
}