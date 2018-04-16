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


class Die{

    public:

    Die();
    Die(int offset, int rolls, int range);

    int rolls;
    int offset;
    int range;
};

enum ItemType{
    INVALID,
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
    CONTAINER,
    STACK
};

class ItemDescription{

    public:

    std::string name;
    std::string description;
    ItemType type;
    int color;
    Die hitBonus;
    Die damageBonus;
    Die defenseBonus;
    Die weight;
    Die speedBonus;
    Die dodgeBonus;
    Die specialAttribute;
    Die value;
    bool isArtifact;
    int rarity;

};

class Item{

    public:

    Item();
    Item(ItemDescription);

    std::string name;
    std::string description;
    ItemType type;
    char sym;
    int color;
    int hitBonus;
    int damageBonus;
    int defenseBonus;
    int weight;
    int speedBonus;
    int dodgeBonus;
    int specialAttribute;
    int value;
    bool isArtifact;
    int rarity;

    //for on map view
    int x, y;
};

class ItemRegistry{
    public:
    std::vector <ItemDescription> registry;
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

//equiptment slots
enum EquiptSlots{
    _WEAPON,
    _OFFHAND,
    _ARMOR,
    _HELM,
    _CLOAK,
    _GLOVES,
    _BOOTS,
    _AMULET,
    _LIGHT,
    _RING0,
    _RING1,
    _CARRY0,
    _CARRY1,
    _CARRY2,
    _CARRY3,
    _CARRY4,
    _CARRY5,
    _CARRY6,
    _CARRY7,
    _CARRY8,
    _CARRY9
};

class Player: public Entity{

    public:

    Player(int x, int y);
    ~Player();

    unsigned * playerMap;

    //slots
    Item weapon;
};


class MonsterRegistry{

    public:
    std::vector <MonsterDescription> registry;

};

extern MonsterRegistry _monsterReg;
extern ItemRegistry _itemReg;

//Entity * createEntity(char sym, int x, int y, int isMonster);
void destroyEntity(Entity * e);
bool hasAttribute(Entity * e, unsigned int);
void setAttribute(Entity * e, unsigned int);

int rollDice(Die d);

void loadTestItems();

#endif