#ifndef ENTITY
#define ENTITY

typedef struct Entity{
    char canTunnel;
    int x, y;
} Entity;

Entity * createEntity(char ct, int x, int y);
void destroyEntity(Entity * e);

#endif