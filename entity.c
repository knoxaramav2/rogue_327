#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "entity.h"
#include "algo.h"

Config config;

Entity * createEntity(char sym, int x, int y, int isMonster){
    Entity * ret = malloc(sizeof(Entity));

    ret->x = x;
    ret->y = y;

    ret->symbol = sym;
    ret->attributes = 0;
    ret->speed = 10;

    if (isMonster){
        ret->attributes |= rand()%2==0 ? INTELLIGENCE : 0;
        ret->attributes |= rand()%2==0 ? TELEPATHY : 0;
        ret->attributes |= rand()%2==0 ? TUNNELABLE : 0;
        ret->attributes |= rand()%2==0 ? ERRATIC : 0;
        ret->speed = randIn(5, 20);
    }

    return ret;
}

void destroyEntity(Entity * e){
    free(e);
}

int hasAttribute(Entity * e, unsigned char attr){
    return e->attributes & attr;
}