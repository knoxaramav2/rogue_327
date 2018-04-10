#ifndef ENTITY
#define ENTITY

#define INTELLIGENCE 1
#define TELEPATHY    2
#define TUNNELABLE   4
#define ERRATIC      8

#include <string>
#include <vector>

struct Die{
    int rolls;
    int offset;
    int range;
};

class Entity{

    public:

    Entity(char sym, int x, int y);
    //~Entity();

    char symbol;
    int color;
    int x, y;
    int lastX, lastY;
    int lastPcX, lastPcY;

    //monster attributes
    unsigned char attributes;
    char speed;
    int health;
    Die attack;
    int rarity;
    std::vector<int> colors;
};

class Player: public Entity{

    public:

    Player(int x, int y);
    ~Player();

    unsigned * playerMap;
};

class MonsterDefinition{

    public:
    //monster attributes
    std::string name;
    unsigned char attributes;
    Die speed;
    Die health;
    Die attack;
    int rarity;
    std::string description;
    std::vector<int> colors;
};


class MonsterRegistry{

    public:
    std::vector <MonsterDefinition> registry;

};

extern MonsterRegistry _monsterReg;

//Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
int hasAttribute(Entity * e, unsigned char);

#endif