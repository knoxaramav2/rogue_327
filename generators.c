#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "dungeon.h"

Dungeon * generateDungeon(){

    Dungeon * dungeon = malloc(sizeof(Dungeon));
    dungeon->screen = malloc((DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));
    memset(dungeon->screen, ROCK_SPACE, (DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));//generate empty dungeon

    
    //generate locations
    int numRooms = rand() % MAX_ROOM + MIN_ROOM;
    char ** locations = malloc(sizeof (char *));

    //generate room meta
    int i = 0;
    for (; i < numRooms;  ++i){
        locations[i] = malloc(3);
        locations[i][0] = (rand()%(DUNGEON_WIDTH-6)) + 3;
        locations[i][1] = (rand()%(DUNGEON_HEIGHT-6)) + 3;
        locations[i][2] = (rand()%8)+3;

        printf("At %d %d x %d \r\n", locations[i][0], locations[i][1], locations[i][2]);

        dungeon->screen[CELL(locations[i][0], locations[i][1])] = '$';
    }

    //create rooms from meta
    for (i=0; i < numRooms; ++i){
        int x = locations[i][0];
        int y = locations[i][1];
        int s = locations[i][2];

        int xMin = (x - s/2) < 0 ? 0 : x - s/2;
        int xMax = (x + s/2) < 0 ? DUNGEON_WIDTH-1 : x + s/2;
        int yMin = (y - s/2) < 0 ? 0 : y - s/2;
        int yMax = (y + s/2) < 0 ? DUNGEON_HEIGHT-1 : y + s/2;

        printf("%d - %d x %d - %d\r\n", xMin, xMax, yMin, yMax);

        int h, v;
        for (h=xMin; h<xMax; ++h){
            for (v=yMin; v<yMax; ++v){
                dungeon->screen[CELL(h, v)] = OPEN_SPACE;
            }

            //renderScreen(dungeon);
            //fflush(stdout);
        }
    }

    

    return dungeon;
}
