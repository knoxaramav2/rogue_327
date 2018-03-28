#ifndef ENTITY
#define ENTITY

#define INTELLIGENCE 1
#define TELEPATHY    2
#define TUNNELABLE   4
#define ERRATIC      8

#include <string>
#include <vector>

struct Die{
    int min;
    int max;
};

class Entity{

    public:

    Entity(char sym, int x, int y);
    //~Entity();

    char symbol;
    int color;
    int x, y;
    int lastPcX, lastPcY;

    //monster attributes
    unsigned char attributes;
    char speed;
    int health;
    int attack;
    int rarity;
    std::string description;
    std::vector<std::string> colors;

    //For templates
    Die _speed_;
};

class Player: public Entity{

    public:

    Player(int x, int y);
    ~Player();

    unsigned * playerMap;
};

class EntityRegistry{

    public:
    std::vector <Entity> registry;

};

//Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
int hasAttribute(Entity * e, unsigned char);

#endif