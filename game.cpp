#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "termios.h"

#include "_win_unix.h"

#include "algo.h"
#include "game.h"
#include "config.h"
#include "console.h"
#include "defs.h"

//Config config;
TurnItem * queue = nullptr;
int turn = 0;

#define STATE_NORMAL 0
#define STATE_REGEN  1

int STATE_FLAG = STATE_NORMAL;

int getInput(){

    while(1){
        char c = getch();
        char c2 = 0;
        char c3 = 0;

        //printf("<<%d %d %d>>\r\n", c, c2, c3);
        //fflush(stdout);

        switch(c){
            case '7':
            case 'y':
                return TOP_LEFT;
            case '8':
            case 'k':
                return TOP_MIDDLE;
            case '9':
            case 'u':
                return TOP_RIGHT;
            case '6':
            case 'l':
                return MIDDLE_RIGHT;
            case '3':
            case 'n':
                return BOTTOM_RIGHT;
            case '2':
            case 'j':
                return BOTTOM_MIDDLE;
            case '1':
            case 'b':
                return BOTTOM_LEFT;
            case '4':
            case 'h':
                return MIDDLE_LEFT;

            case '>':
            case '.':
                return MOVE_LOWER;
            case ',':
            case '<':
                return MOVE_UPPER;
            case '5':
                return REST_TURN;
            
            case 'm':
                return LIST_MONSTERS;

            case 27:
                return SCROLL_UP;
            case 28:
                return SCROLL_DOWN;

            case 91:
                return ESCAPE;

            case 'q':
            case 'Q':
                return QUIT_GAME;
        }
    }

    return INVALID_KEY;
}

void tunnelAt(Dungeon * d, int coord){
    int dbHardness = getHardness(d->screen[coord]);
    int hardness = getHardness(d->screen[coord]) - 85;
    if (hardness <= 0){
        hardness = 0;
        d->screen[coord]=setSymbol(d->screen[coord], HALL_SPACE);
    }

    //d->screen[coord]=setSymbol(d->screen[coord], HALL_SPACE);
    d->screen[coord]=setHardness(d->screen[coord], (unsigned) hardness);

    //printf("Tunnel: %u => %u\r\n", dbHardness, getHardness(d->screen[coord]));
}

Entity * attack(Entity * attacker, Dungeon *d){
    Entity * victim = 0;


    return victim;
}

//return 1 if player seen
int lineOfSight(Entity * e, Dungeon * d){

    
    return 0;
}

void moveByStrategy(Entity * e, Dungeon * d, int * toX, int * toY){
    int intelligent = hasAttribute(e, INTELLIGENCE);
    int telepathy = hasAttribute(e, TELEPATHY);
    int tunnel = hasAttribute(e, TUNNELABLE);
    int erratic = hasAttribute(e, ERRATIC);

    //temorary for error checking positions
    int _x = *toX, _y = *toY;

}

void updatePcMap(Dungeon * d){

    Player * p = d->player;
    unsigned * screen = d->screen;
    unsigned * pcScreen = p->playerMap;

    int minX = p->x >= 2  ? p->x - 2: 0;
    int minY = p->y >= 2  ? p->y - 2: 0;
    int maxX = p->x <= DUNGEON_WIDTH - 2 ? p->x + 2: DUNGEON_WIDTH;
    int maxY = p->y <= DUNGEON_HEIGHT - 2 ? p->y + 2: DUNGEON_HEIGHT;

    for (int x = minX; x < maxX; ++x){
        for (int y = minY; y < maxY; ++y){
            int idx = CELL(x, y);
            p->playerMap[idx] = screen[idx];
        }
    }

}

void allowMove(Entity * e, Dungeon ** d){

    int fromX, fromY;
    int toX, toY;

    fromX = e->x;
    fromY = e->y;
    toX = fromX;
    toY = fromY;

    if (e == (*d)->player){

        int acceptKey;
        int _x = e->x, _y = e->y;
        int action = -1;

        do{
            acceptKey = 1;
            fflush(stdin);
            //char c = getchar();

            int kc = getInput();

            //printf("<%d>\r\n", kc);

            switch(kc){
                case BOTTOM_LEFT: 
                    --_x;
                    ++_y;
                break;
                case BOTTOM_MIDDLE: 
                    ++_y;
                break;
                case BOTTOM_RIGHT: 
                    ++_x;
                    ++_y;
                break;
                case MIDDLE_LEFT: 
                    --_x;
                break;
                case MIDDLE_RIGHT: 
                    ++_x;
                break;
                case TOP_LEFT: 
                    --_x;
                    --_y;
                break;
                case TOP_MIDDLE: 
                    --_y;
                break;
                case TOP_RIGHT: 
                    ++_x;
                    --_y;
                break;

                case REST_TURN: break;

                case MOVE_UPPER:
                case MOVE_LOWER:

                {
                char stairSym = MOVE_UPPER ? '<' : '>';

                queue = nullptr;

                *d = generateDungeon(*d);
                setupGameState(*d);
                spawnPlayers(*d);
                calcDistMap(*d, 0);

                STATE_FLAG = STATE_REGEN;

                return;

                //regenerate level
                //place player on staircase

                }

                break;

                case QUIT_GAME: config._run = 0; return;
                default: acceptKey = 0;
            }

            if (!isValidSpace(_x, _y)){
                _x = e->x;
                _y = e->y;
                acceptKey = 0;
            }

        } while(!acceptKey);

        //check for obstruction
        if (getSymbol((*d)->screen[CELL(_x, _y)]) == ROCK_SPACE){
            tunnelAt(*d, CELL(_x, _y));
        } else {

            toX = _x;
            toY = _y;
        }

    } else {
        moveByStrategy(e, *d, &toX, &toY);   
    }

    Entity * victim = attack(e, *d);

    //printf("%c [%d,%d] => [%d,%d]\r\n", e->symbol, fromX, fromY, toX, toY);
    e->x = toX;
    e->y = toY;

    return;
}

void printQueue(){
    TurnItem * ti = queue;
    while(ti){
        //printf("[%c %d] ", ti->e->symbol, ti->val);
        ti = ti->next;
    }

    //printf("\r\n");
    //fflush(stdout);
}

void queueTurn(Entity * e, int val){
    TurnItem * ti = (TurnItem*) malloc(sizeof(TurnItem));
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
    //system ("/bin/stty raw");
}

void spawnPlayers(Dungeon * d){
    //randomly place monster
    int i;
    for (i=0; i<config.numNpc; ++i){
        int mx=-1, my;
        do{
            mx = randIn(1, DUNGEON_WIDTH-1);
            my = randIn(1, DUNGEON_HEIGHT-1);

            char open = getSymbol(d->screen[CELL(mx, my)]);
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

        d->npcs[i] = new Entity(sym, mx, my);//createEntity(sym, mx, my, 1);
        queueTurn(d->npcs[i], 1000/d->npcs[i]->speed);
    }

    //place player in room
    int rId = randIn(0, d->roomCount);
    int * roomSet = d->roomInfo[rId];
    int xLoc = roomSet[0] + (roomSet[2] / 2 );
    int yLoc = roomSet[1] + (roomSet[3] / 2);

    d->player = new Player(xLoc, yLoc);//createEntity('@', xLoc, yLoc, 0);
    queueTurn(d->player, 100);
}

void updateTurn(Dungeon ** d){
    ++turn;

    if (turn >= queue->val){
        Entity * e = queue->e;
        int val = queue->val;

        //printf("Turn %d\r\n", turn);
        updateScreen(*d);
        allowMove(e, d);
        updateScreen(*d);
        //renderScreen(*d);
        updatePcMap(*d);

        if (STATE_FLAG == STATE_REGEN){
            STATE_FLAG = STATE_NORMAL;
            turn = 0;
            return;
        }

        
        pop();
        queueTurn(e, val + (1000/e->speed));

        //sleep(1);
    }
}