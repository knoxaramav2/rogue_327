#include <string.h>
#include <stdlib.h>

#include "entity.h"

Entity * createEntity(char ct, int x, int y){
    Entity * ret = malloc(sizeof(Entity));

    ret->canTunnel = ct;
    ret->x = x;
    ret->y = y;

    return ret;
}

void destroyEntity(Entity * e){
    free(e);
}