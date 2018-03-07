#ifndef ENTITY
#define ENTITY

#define INTELLIGENCE 1
#define TELEPATHY    2
#define TUNNELABLE   4
#define ERRATIC      8

typedef struct Entity{

    char symbol;
    int x, y;
    int lastPcX, lastPcY;

    //monster attributes
    unsigned char attributes;
    char speed;
} Entity;

Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
int hasAttribute(Entity * e, unsigned char);

#endif