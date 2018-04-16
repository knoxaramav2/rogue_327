#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include <string>
#include <vector>

using namespace std;

//#include "_win_unix.h"

#include "algo.h"
#include "game.h"
#include "config.h"
#include "console.h"
#include "defs.h"
#include "entity.h"

//MonsterRegistry _monsterReg;

//Config config;
TurnItem * queue = nullptr;
int turn = 0;

#define STATE_NORMAL    0
#define STATE_REGEN     1
#define STATE_NON_MOVE  2

int STATE_FLAG = STATE_NORMAL;

int rollDice(Die d){
    int sum = d.offset;

    for (int i = 0; i < d.rolls; ++i){
        sum += randIn(1, d.range);
    }

    return sum;
}

int getInput(){

    while(1){
        int c = wgetch(__console.gameWindow);
        wrefresh(__console.gameWindow);

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

            case 27:{
                int k = wgetch(__console.gameWindow);
                
                if (k == 91){
                    k = wgetch(__console.gameWindow);
                    if (k==65) return UP_ARROW;
                    if (k==66) return DOWN_ARROW;
                    if (k==67) return RIGHT_ARROW;
                    if (k==68) return LEFT_ARROW;
                } else if (k == 27){
                    return ESCAPE;
                }
            }
            return INVALID_KEY;
            case 91:
                return ESCAPE;

            case 'q':
            case 'Q':
                return QUIT_GAME;
            case 'f':
                return FOG_OF_WAR;
            case 't':
                return TELEPORT;
            case 'r':
                return RANDOM;
        }
    }

    return INVALID_KEY;
}

void monsterDisplayMode(Dungeon * d){

    //while(true){};

    int scrollPos = 0;
    vector <string> list;

    for (int i = 0; i < config.numNpc; ++i){
        list.push_back(
            getRelativeString(
                d->player,
                d->npcs[i]
            ));
    }

    while(true){
        __console.displayMonsterStats(list, scrollPos);

        int i = getInput();

        switch(i){
            case UP_ARROW: --scrollPos; break;
            case DOWN_ARROW: ++scrollPos; break;
            case ESCAPE: 
                renderMap(getActiveMap(d));
            return;
        }

    }
}

void teleportMode(Dungeon * d){

    char c;

    Player * p = d->player;
    unsigned * map = getActiveMap(d);
    int x = p->x;
    int y = p->y;
    int _x = x;
    int _y = y;

    char lastElement;
    bool wait = true;
    bool random = false;

    do{
        lastElement = getSymbol(map[CELL(x, y)]);

        char result = getInput();

        switch(result){

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

            case TELEPORT:
                wait = false;
            break;
            case RANDOM:
                wait=false;
                random = true;
            break;
        }

        mvwprintw(__console.gameWindow, y, x, "%c", lastElement);
        mvwprintw(__console.gameWindow, _y, _x, "*");
        y = _y;
        x = _x;

    } while(wait);

    //repair
    lastElement = mvwinch(__console.gameWindow, y, x) & A_CHARTEXT;
    mvwprintw(__console.gameWindow, p->y, p->x, "%c", lastElement);

    int toX = x;
    int toY = y;

    if (random){
        toX = randIn(1, DUNGEON_WIDTH-2);
        toY = randIn(1, DUNGEON_HEIGHT-2);
    }

    p->x = toX;
    p->y = toY;

    mvwprintw(__console.gameWindow, p->y, p->x, "%c", "@");
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
    Entity * victim = NULL;


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

    //get shortest distance from player
    int shortCoord = getIndex(_x, _y);
    int shortVal = d->_distanceMap[shortCoord];

    for (int i = BOTTOM_LEFT; i <= TOP_RIGHT; ++i){
        int currentCoord = getAdjacentIndex(_x, _y, i);
        int currentVal = d->_distanceMap[currentCoord];
        if (currentVal < shortVal){
            shortVal = currentVal;
            shortCoord = currentCoord;
        }
    }

    *toX = getX(shortCoord);
    *toY = getY(shortCoord);
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

                case FOG_OF_WAR:
                    (*d)->fogOfWar = !(*d)->fogOfWar; 
                    renderMap((*d)->fogOfWar ? 
                        (*d)->player->playerMap :
                        (*d)->screen);
                    STATE_FLAG = STATE_NON_MOVE;
                    break;
                case TELEPORT: teleportMode(*d); return;
                case LIST_MONSTERS:
                    STATE_FLAG = STATE_NON_MOVE;
                    monsterDisplayMode((*d));
                    return;
                break;

                default: acceptKey = 0;
            }

            if (!isValidSpace(_x, _y)){
                _x = e->x;
                _y = e->y;
                acceptKey = 0;
            }

        } while(!acceptKey);

        //check for obstruction
        if (getSymbol((*d)->screen[CELL(_x, _y)]) != ROCK_SPACE){
            //tunnelAt(*d, CELL(_x, _y));
            toX = _x;
            toY = _y;
        }

        calcDistMap(*d, true);

    } else {
        moveByStrategy(e, *d, &toX, &toY);
    }

    Entity * victim = attack(e, *d);

    e->lastX = e->x;
    e->lastY = e->y;
    e->x = toX;
    e->y = toY;

    updateMove(*d, e);

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

        MonsterDescription def = 
            _monsterReg.registry[randIn(0, _monsterReg.registry.size())];
        
        //determine npcs type
        char sym = def.name[0];
        //printf("%c at %d %d\r\n", sym, mx, my);
        d->npcs[i] = new Entity(sym, mx, my);//createEntity(sym, mx, my, 1);
        
        d->npcs[i]->colors = def.colors;
        d->npcs[i]->attributes = def.attributes;
        d->npcs[i]->speed = rollDice(def.speed);
        d->npcs[i]->health = rollDice(def.speed);
        d->npcs[i]->attack = def.attack;
        d->npcs[i]->rarity = def.rarity;

        
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

void spawnItems(Dungeon * d){
    int numItems = randIn(10, 15);

    //spawn items
    for (int i = 0; i < numItems; ++i){
        Item item(_itemReg.registry[randIn(0, _itemReg.registry.size()-1)]);

        //place items
        while(true){
            int x = randIn(1, DUNGEON_WIDTH-1);
            int y = randIn(1, DUNGEON_HEIGHT-1);
            int cell = CELL(x,y);

            if (getSymbol(d->screen[cell]) != ROCK_SPACE){
                item.x = x;
                item.y = y;
                break;
            }
        }

        d->items.push_back(item);
    }
}

void updateTurn(Dungeon ** d){
    ++turn;

    if (turn >= queue->val){
        Entity * e = queue->e;
        int val = queue->val;

        if (e == (*d)->player){
            updateScreen(*d);
            updatePcMap(*d);
        }

        allowMove(e, d);

        if (STATE_FLAG == STATE_REGEN){
            STATE_FLAG = STATE_NORMAL;
            turn = 0;
        } else if (STATE_FLAG == STATE_NON_MOVE){
            STATE_FLAG = STATE_NORMAL;
            --turn;
        } else {
            pop();
            queueTurn(e, val + (1000/e->speed));
        }

        updateScreen(*d);
        

        //sleep(1);
    }
}