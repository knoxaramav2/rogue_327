#ifndef ENTITY
#define ENTITY

#define INTELLIGENCE    1
#define TELEPATHY       2
#define TUNNELABLE      4
#define ERRATIC         8
#define PASS            16
#define PICKUP          32
#define DESTROY         64
#define UNIQ            128
#define BOSS            256

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
    unsigned int attributes;
    char speed;
    int health;
    Die attack;
    int rarity;
    std::vector<int> colors;
};

class MonsterDescription{

    public:
    //monster attributes
    std::string name;
    char sym;
    unsigned int attributes;
    Die speed;
    Die health;
    Die attack;
    int rarity;
    std::string description;
    std::vector<int> colors;
};

class Monster: public Entity{

    public:

    Monster(int x, int y, MonsterDescription md);
    std::string description;
};

class Player: public Entity{

    public:

    Player(int x, int y);
    ~Player();

    unsigned * playerMap;
};


class MonsterRegistry{

    public:
    std::vector <MonsterDescription> registry;

};


enum ItemType{
    WEAPON,
    OFFHAND,
    RANGED,
    ARMOR,
    HELMET,
    CLOAK,
    GLOVES,
    BOOTS,
    RING,
    AMULET,
    LIGHT,
    SCROLL,
    BOOK,
    FLASK,
    GOLD,
    AMMUNITION,
    FOOD,
    WAND,
    CONTAINER
};

class ItemDescription{

    public:

    Item(ItemDescription);

    std::string name;
    std::string description;
    ItemType type;
    int color;
    Die hitBonus;
    Die damageBonus;
    Die defenseBonus;
    Die weight;
    Die speedBonus;
    Die specialAttribute;
    Die value;
    bool isArtifact;
    int rarity;

};

class Item{

    public:

    Item(ItemDescription);

    std::string name;
    std::string description;
    ItemType type;
    int color;
    int hitBonus;
    int damageBonus;
    int defenseBonus;
    int weight;
    int speedBonus;
    int specialAttribute;
    int value;
    bool isArtifact;
    int rarity;

};

class ItemRegistry{
    public:
    std::vector <ItemDescription> registry;
};

extern MonsterRegistry _monsterReg;
extern ItemRegistry _itemReg;

//Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
bool hasAttribute(Entity * e, unsigned int);
void setAttribute(Entity * e, unsigned int);

int rollDice(Die d);

#endif