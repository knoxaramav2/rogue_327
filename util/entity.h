#ifndef ENTITY
#define ENTITY

#define INTELLIGENCE 1
#define TELEPATHY    2
#define TUNNELABLE   4
#define ERRATIC      8

class Entity{

    public:

    Entity(char sym, int x, int y);
    //~Entity();

    char symbol;
    int x, y;
    int lastPcX, lastPcY;

    //monster attributes
    unsigned char attributes;
    char speed;
};

class Player: public Entity{

    public:

    Player(int x, int y);
    ~Player();

    unsigned * playerMap;
};

//Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
int hasAttribute(Entity * e, unsigned char);

#endif