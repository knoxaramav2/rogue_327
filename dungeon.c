#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//size
#define DUNGEON_HEIGHT  21
#define DUNGEON_WIDTH   80
#define MIN_ROOM        5
#define MAX_ROOM        7

//legend
#define OPEN_SPACE      '@'
#define ROCK_SPACE      ' '
#define HALL_SPACE      '#'

//bit masks
#define SYMBOL_MASK     0x0000FFFF
#define HARD_MASK       0x000F0000
#define MUTABLE_MASK    0x00F00000

//UTIL
#define CELL(x,y) ((x%DUNGEON_WIDTH) + (y*DUNGEON_WIDTH))

//Generators
typedef struct Dungeon{
    unsigned * screen;
} Dungeon;

Dungeon * generateDungeon(){
    Dungeon * dungeon = malloc(sizeof(Dungeon));
    dungeon->screen = malloc((DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));
    memset(dungeon->screen, OPEN_SPACE, (DUNGEON_WIDTH * DUNGEON_HEIGHT) * sizeof(unsigned));//generate empty dungeon

    
    //generate locations
    int numRooms = rand() % MAX_ROOM + MIN_ROOM;
    char ** locations = malloc(numRooms);

    //allocate room memory
    int i = 0;
    for (; i < numRooms; ++i){
        locations[i] = malloc(3);
    }

    //create rooms
    i = 2;
    int j = 0;
    for (; i < numRooms && j < numRooms; ++i){
        if ((i%DUNGEON_WIDTH) < 3 || (i % DUNGEON_WIDTH) > DUNGEON_WIDTH - 3 || 
            (i%DUNGEON_WIDTH) / 3 || (i % DUNGEON_WIDTH) / DUNGEON_WIDTH - 3){
                continue;
            }
        
        
    }

    //free room memory
    i = 0;
    for (; i < numRooms; ++i){
        free(locations[i]);
    }

    return dungeon;
}

//Renderers
int clearScreen(){
    //TODO use ascii escape method instead
    int i = 0;
    for (; i < 30; ++i){
        printf("\n");
    }

    printf("\r");
}

int renderScreen(Dungeon * dungeon){
    //TODO stop clearing each update
    clearScreen();

    int i = 0;
    for (; i < DUNGEON_WIDTH * DUNGEON_HEIGHT; ++i){
        if (i % DUNGEON_WIDTH == 0){
            printf("\r\n");
        }
        int cell = (i%(DUNGEON_WIDTH)) + ((i/DUNGEON_WIDTH) * DUNGEON_WIDTH);
        printf("%c", (char) dungeon->screen[i]);
    }

    //render text below
    printf("\n|\n|\n|\r\n");
}

//Attributes
int getHardness(int cell){
    return cell & HARD_MASK;
}

int setHardness(int cell, int hardness){
    return ((cell & SYMBOL_MASK) | (cell & MUTABLE_MASK) & (cell & !HARD_MASK)) | hardness;
}

int getMutable(int cell){

}

int setMutable(int cell, int mutable){

}

char getSymbol(int cell){

}

char setSymbol(int cell, char symbol){
    
}

int main(int argc, char ** argv){

    Dungeon * dungeon = generateDungeon();

    int i = 0;
    for (; i < 20; ++i) dungeon->screen[CELL(i,i)] = '#';

    renderScreen(dungeon);

    return 0;
}