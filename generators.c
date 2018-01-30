#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "dungeon.h"
#include "algo.h"
#include "math.h"

#include "console.h"


typedef struct Coord{
    int x, y;
} Coord;

typedef struct CoordStack{
    int (*coord)[2];
    int size;
    int index;
} CoordStack;

CoordStack * createCoordStack(){
    CoordStack * cs = malloc(sizeof(CoordStack));

    const int defaultSize = 10;

    cs->size = defaultSize;
    cs->index = 0;
    cs->coord = malloc(sizeof(int[2]) * defaultSize);

    return cs;
}

void freeCoordStack(CoordStack * cs){
    free(cs->coord);
    free(cs);
}

Coord popCoordStack(CoordStack * cs){

    Coord c;

    if (cs->index == 0){
        c.x = -1;
        c.y = -1;
        return c;
    }

    c.x = cs->coord[--cs->index][0];
    c.y = cs->coord[  cs->index][1];

    return c;
}

void pushCoordStack(CoordStack * cs, int coord[2]){
    
    if (cs->index == cs->size){
        cs->size *= 2;
        cs->coord = realloc(cs->coord, sizeof(int[2]) * cs->size);
    }

    cs->coord[cs->index][0] = coord[0];
    cs->coord[cs->index][1] = coord[1];
    ++cs->index;
}

int checkRoomCollide(int ** rooms, int curr){

    int idx = curr - 1;

    int x0 = rooms[curr][0] - 1;
    int y0 = rooms[curr][1] - 1;
    int x1 = rooms[curr][2] + x0 + 1;
    int y1 = rooms[curr][3] + y0 + 1;

    for (; idx >= 0; --idx){
        int i0 = rooms[idx][0];
        int j0 = rooms[idx][1];
        int i1 = rooms[idx][2] + i0;
        int j1 = rooms[idx][3] + j0;

        if (((x0 > i1 || i0 > x1) || (y0 > j1 || j0 > y1)) == 0)
            return 1;
    }

    return 0;
}

//generates set of rooms, attempts to place them
//CONSTRAINTS: 100 placement attempts per room before re-shuffle
//5 reshuffles before giving up
int calculateRooms(Dungeon * dungeon){

    int numRooms = randIn(MIN_ROOM, MAX_ROOM);
    dungeon->roomInfo = malloc(sizeof(int *) * numRooms);
    int i = 0;
    int roomAttempts = 0;
    int shuffleAttempts = 0;

    for(; i < numRooms; ++i){
        dungeon->roomInfo[i] = malloc(sizeof(int) * 4);
        int * t = dungeon->roomInfo[i];
        
        roomAttempts = 0;
        while(roomAttempts < 100){
            //choose room size
            dungeon->roomInfo[i][2] = randIn(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
            dungeon->roomInfo[i][3] = randIn(MIN_ROOM_SIZE, MAX_ROOM_SIZE);

            //choose room location within possible areas
            dungeon->roomInfo[i][0] = randIn(1, (DUNGEON_WIDTH - 1) - dungeon->roomInfo[i][2]);
            dungeon->roomInfo[i][1] = randIn(1, (DUNGEON_HEIGHT - 1) - dungeon->roomInfo[i][3]);

            if (checkRoomCollide(dungeon->roomInfo, i) == 0){
                break;
            } else {
                ++roomAttempts;
                continue;
            }
        }

        if (roomAttempts == 100){
            ++shuffleAttempts;
            
            //Free memory, give up
            if (shuffleAttempts == 5){
                int j = 0;
                for (; j < i; ++ j) {free (dungeon->roomInfo[i]);}
                free(dungeon->roomInfo);
                return 0;
            }
        }
    }
    
    return numRooms;
}

/*
    Corridor generation

    1) From list, choose host incrementally and target randomly until all rooms have at least one connection
    2) Mark a 'door' location randomly on the outside of each, where possible
    3) Randomly choose a starting direction, not towards the host
    4) Move in chosen direction until a non-rock face is met
    5) Randomly choose a direction on the opposite axis of movement, and save the coordinate
    6) Continue until it is possible to resume the original direction, and save the coordinate
    7) If a border wall is met and it is impossible to continue, backtrack to last coordinate
       and attempt the opposite direction. If this too fails, retry with a new target. Allow for 
       as many attempts as there are rooms before moving on.
    8) When the nearest wall face is met on the axis of movement, begin moving flush in its direction
       until perpendicular with the door. Save the coordinate.
    9) Continue to use above rules, saving each time an obstacle is faced, backtracking when unable to
       continue, ect, until the door is reach. Generate a pathway between each saved coordinate 
*/
int generateCorridors(Dungeon * d){

    char * hasConnection = malloc(d->roomCount);
    memset(hasConnection, 0, d->roomCount);
    int doorLoc [2][2] = {{-1,-1}, {-1,-1}};
    int rIdx = 0;
    int hIdx = 0;

    CoordStack * cs;

    int complete = 0;

    while (!complete){

        //get host, target
        int * host;
        int * target;

        host = d->roomInfo[rIdx];
        do{
            hIdx = randIn(0, d->roomCount-1);
            target = d->roomInfo[hIdx];
        } while(target == host);

        //mark a door location on each
        {
            int x, y;
            unsigned cell;

            //host
            do{
            x = randIn(host[0], host[0] + host[2]);
            y = randIn(host[1], host[1] + host[3]);
            int dir = rand()%4;

            switch(dir){
            case 0: x = host[0] - 1;                break;
                case 1: x = host[0] + host[2] + 1;  break;
                case 2: y = host[1] - 1;            break;
                case 3: y = host[1] + host[3] + 1;  break;
            }

            doorLoc[0][0] = x;
            doorLoc[0][1] = y;

            cell = d->screen[CELL(x, y)];
            }while (getImmutable(cell) == 1 && getSymbol(cell) != OPEN_SPACE);

            //target
            do{
            x = randIn(target[0], target[0] + target[2]);
            y = randIn(target[1], target[1] + target[3]);
            int dir = rand()%4;

            switch(dir){
                case 0: x = target[0] - 1;              break;
                case 1: x = target[0] + target[2] + 1;  break;
                case 2: y = target[1] - 1;              break;
                case 3: y = target[1] + target[3] + 1;  break;
            }

            doorLoc[1][0] = x;
            doorLoc[1][1] = y;

            cell = d->screen[CELL(x, y)];
            }while (getImmutable(cell) == 1 && getSymbol(cell) != HALL_SPACE);
        }

        //Begin mapping cooridor
        cs = createCoordStack();

        //add source to stack
        pushCoordStack(cs, doorLoc[0]);


        //add destination to stack
        pushCoordStack(cs, doorLoc[1]);

        //build corridor
        Coord current = popCoordStack(cs);
        Coord next = popCoordStack(cs);

        do{

            int fromx, fromy, tox, toy;
            int x, y;

            if (current.x < next.x){
                fromx = current.x;
                tox = next.x;
            } else {
                fromx = next.x;
                tox = current.x;
            }

            if (current.y < next.x){
                fromy = current.y;
                toy = next.y;
            } else {
                toy = current.y;
                fromy = next.y;
            }

            x = fromx;
            y = fromy;

            for (; x < tox; ++x){
                if (getSymbol(d->screen[CELL(x, y)]) != OPEN_SPACE)
                    d->screen[CELL(x, y)] = setHardness(HALL_SPACE, 0);
            }

            for (--x; y < toy; ++y){
                if (getSymbol(d->screen[CELL(x, y)]) != OPEN_SPACE)
                    d->screen[CELL(x, y)] = setHardness(HALL_SPACE, 0);
            }

            current = next;
            next = popCoordStack(cs);
        } while (next.x != -1);

        hasConnection[rIdx] = 1;
        hasConnection[hIdx] = 1;

        //reset room counter
        if (++rIdx == d->roomCount){
            rIdx = 0;
        }

        //check if all rooms are connected
        {
            int i = 0;
            complete = 1;
            for (; i < d->roomCount; ++ i){
                if (hasConnection[i] == 0){
                    complete = 0;
                    break;
                }
            }
        }
    }


    return 0;
}

Dungeon * generateDungeon(){
    //Generate rock map
    Dungeon * dungeon = malloc(sizeof(Dungeon));
    dungeon->screen = malloc((DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));
    memset(dungeon->screen, setHardness(ROCK_SPACE, 3), (DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));//generate empty dungeon
    dungeon->roomCount = 0;
    dungeon->roomInfo = 0;

    //set immutable border
    int bx = 0;
    for (; bx < DUNGEON_WIDTH; ++bx){
        dungeon->screen[CELL(bx, 0)] = ROCK_SPACE | IMMUTABLE_MASK;
        dungeon->screen[CELL(bx, (DUNGEON_HEIGHT-1))] = ROCK_SPACE | IMMUTABLE_MASK;

        if (bx < DUNGEON_HEIGHT){
            dungeon->screen[CELL(0, bx)] = ROCK_SPACE | IMMUTABLE_MASK;
            dungeon->screen[CELL(DUNGEON_WIDTH-1, bx)] = ROCK_SPACE | IMMUTABLE_MASK;
        }
    }

    //Attempt to generate rooms
    dungeon->roomCount = calculateRooms(dungeon);
    if (dungeon->roomCount == 0){
        free(dungeon);
        return 0;
    }

    //Apply rooms to map
    int i = 0;
    for (; i < dungeon->roomCount; ++i){
        int * cr = dungeon->roomInfo[i];
        int x = cr[0];
        int y = cr[1];
        int xx = x + cr[2];
        int yy = y + cr[3];

        printf("%d x %d\r\n", cr[2], cr[3]);

        for (; x < xx; ++x){
            for (y = cr[1]; y < yy; ++y){
                dungeon->screen[CELL(x, y)] = setHardness(OPEN_SPACE, 0);
            }
        }
    }

    generateCorridors(dungeon);

    //renderScreen(dungeon);
    //fflush(stdout);

    return dungeon;
}
