#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "entity.h"
#include "algo.h"
#include "defs.h"

//Config config;

Entity * createEntity(char sym, int x, int y, int isMonster){
    Entity * ret = (Entity *) malloc(sizeof(Entity));

    ret->x = x;
    ret->y = y;
    ret->lastPcX = -1;
    ret->lastPcY = -1;

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

Entity::Entity(char sym, int x, int y){
    this->x = x;
    this->y = y;
    this->symbol = sym;

    lastPcX = lastPcY = -1;

    attributes = 0;

    attributes |= rand()%2==0 ? INTELLIGENCE : 0;
    attributes |= rand()%2==0 ? TELEPATHY : 0;
    attributes |= rand()%2==0 ? TUNNELABLE : 0;
    attributes |= rand()%2==0 ? ERRATIC : 0;
    speed = randIn(5, 20);

}

Player::Player(int x, int y) : Entity('@', x, y){
    playerMap = new unsigned [DUNGEON_HEIGHT * DUNGEON_WIDTH];
    memset(playerMap, 0, DUNGEON_HEIGHT * DUNGEON_WIDTH);

    speed = 10;
}

Player::~Player(){
    delete playerMap;
}

void destroyEntity(Entity * e){
    free(e);
}

int hasAttribute(Entity * e, unsigned char attr){
    return e->attributes & attr;
}