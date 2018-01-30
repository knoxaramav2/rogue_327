#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "dungeon.h"
#include "console.h"

//UTIL
int main(int argc, char ** argv){

    srand(time(0));

    Dungeon * dungeon = generateDungeon();

    if (dungeon == 0){
        printf("Unable to generate dungeon\r\n");
        return 0;
    }

    //int i = 0;
    //for (; i < 20; ++i) dungeon->screen[CELL(i,i)] = '#';

    renderScreen(dungeon);

    //unsigned i = 'c';
    //i = setHardness(i, 5);
    //i = setImmutable(i, 1);
    //printf("%c %u %u\r\n", getSymbol(i), getHardness(i), getImmutable(i));
    //printf("%x\r\n", i);

    return 0;
}