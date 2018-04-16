#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "config.h"
#include "entity.h"
#include "algo.h"
#include "defs.h"

//Config config;


Die::Die(){
    rolls = 0;
    offset = 0;
    range = 0;
}

Die::Die(int off, int rll, int rng){
    rolls = rll;
    offset = off;
    range = rng;
}


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

Monster::Monster(int x, int y, MonsterDescription md) : Entity(md.sym, x, y){
    symbol = md.sym;
    colors = md.colors;
    attributes = md.attributes;
    speed = rollDice(md.speed);
    health = rollDice(md.health);
    attack = md.attack;
    rarity = md.rarity;
    
    description = md.description;
}

Player::Player(int x, int y) : Entity('@', x, y){
    playerMap = new unsigned [DUNGEON_HEIGHT * DUNGEON_WIDTH];
    memset(playerMap, 0, DUNGEON_HEIGHT * DUNGEON_WIDTH);

    speed = 10;
}

Player::~Player(){
    delete playerMap;
}

Item::Item(){

}

Item::Item(ItemDescription id){

    switch (id.type){
        case INVALID: sym = '*'; break;
        case WEAPON: sym = '|'; break;
        case OFFHAND: sym = ')'; break;
        case RANGED: sym = '}'; break;
        case ARMOR: sym = '['; break;
        case HELMET: sym = ']'; break;
        case CLOAK: sym = '('; break;
        case GLOVES: sym = '{'; break;
        case BOOTS: sym = '\\'; break;
        case RING: sym = '='; break;
        case AMULET: sym = '\"'; break;
        case LIGHT: sym = '_'; break;
        case SCROLL: sym = '~'; break;
        case BOOK: sym = '?'; break;
        case FLASK: sym = '!'; break;
        case GOLD: sym = '$'; break;
        case AMMUNITION: sym = '/'; break;
        case FOOD: sym = ','; break;
        case WAND: sym = '-'; break;
        case CONTAINER: sym = '%'; break;
        case STACK: sym = '&'; break;
    }

    name = id.name;
    description = id.description;
    type = id.type;
    color = id.color;
    hitBonus = rollDice(id.hitBonus);
    damageBonus = rollDice(id.damageBonus);
    defenseBonus = rollDice(id.defenseBonus);
    weight = rollDice(id.weight);
    speedBonus = rollDice(id.speedBonus);
    dodgeBonus = rollDice(id.dodgeBonus);
    specialAttribute = rollDice(id.specialAttribute);
    value = rollDice(id.value);
    isArtifact = id.isArtifact;
    rarity = id.rarity;

}

void destroyEntity(Entity * e){
    free(e);
}

bool hasAttribute(Entity * e, unsigned int attr){
    return e->attributes & attr;
}



//DEBUG
void loadTestItems(){

    MonsterDescription md;
    ItemDescription id;

    md.name = "Junior Barbarian";
    md.sym = 'p';
    md.colors.push_back(COLOR_BLUE);
    md.description = "This is a junior barbarian. He--or is it she? You can't tell for sure-- \
looks like... it should still be in barbarian school. The barbarians are \
putting them in the dungeons young these days. It's wearing dirty, tattered \
cloth armor and wielding a wooden sword. You have a hard time feeling \
intimidated.";
    md.speed = Die(7,1,4);
    md.health = Die(12,2,6);
    md.attack = Die(0,1,4);
    md.rarity = 100;
    md.attributes = INTELLIGENCE;
    _monsterReg.registry.push_back(md);
    
    md = MonsterDescription();

    md.name = "Amazon Lich Queen";
    md.sym = 'q';
    md.colors.push_back(COLOR_BLACK);
    md.description = "This is a junior barbarian. He--or is it she? You can't tell for sure-- \
looks like... it should still be in barbarian school. The barbarians are \
putting them in the dungeons young these days. It's wearing dirty, tattered \
cloth armor and wielding a wooden sword. You have a hard time feeling \
intimidated.";
    md.speed = Die(10,10,2);
    md.attack = Die(30,5,9);
    md.health = Die(2999,1,1001);
    md.rarity = 20;
    md.attributes = INTELLIGENCE | PASS;
    _monsterReg.registry.push_back(md);
    
    md = MonsterDescription();

    id.name = "a NERF(R) dagger";
    id.type = WEAPON;
    id.color = COLOR_MAGENTA;
    id.weight = Die(1,0,1);
    id.hitBonus = Die(0,0,1);
    id.damageBonus = Die(0,0,1);
    id.specialAttribute = Die(0,0,1);
    id.value = Die(9,1,6);
    id.dodgeBonus = Die(10,0,1);
    id.description = "This is a totally wicked looking dagger. It's got awesome barbs on the back \
of the blade, a compass on the hilt, and myriad other embelishments that \
serve no functional purpose. You could totally be a deadly assassin with a \
sweet blade like this. Since it's so light, it won't encumber you. It's \
made out of polyurethane foam.";
    _itemReg.registry.push_back(id);
    id=ItemDescription();

    id.name = "a torch";
    id.type = LIGHT;
    id.color = COLOR_BLACK;
    id.weight = Die(2,1,1);
    id.hitBonus = Die(0,0,1);
    id.damageBonus = Die(0,0,1);
    id.specialAttribute = Die(3,2,3);
    id.value = Die(9,1,6);
    id.dodgeBonus = Die(10,0,1);
    id.description = "From the makers of the world's most refined lasers, comes the ultimate  \
handheld flashlights. The Flashtorch is a compact, portable searchlight that \
is capable of producing an incredible 4100 lumens of intense white light. Use \
this power to guide your way home, light a fire, or even fry an egg!";
    _itemReg.registry.push_back(id);
    id=ItemDescription();

    id.name = "a chainmail coif";
    id.type = HELMET;
    id.color = COLOR_MAGENTA;
    id.weight = Die(1,0,1);
    id.hitBonus = Die(0,0,1);
    id.damageBonus = Die(0,0,1);
    id.specialAttribute = Die(0,0,1);
    id.value = Die(9,1,6);
    id.dodgeBonus = Die(10,0,1);
    id.defenseBonus = Die(20,3,8);
    id.description = "A heavy chainmail head covering.";
    _itemReg.registry.push_back(id);
    id=ItemDescription();

    id.name = "Aegis";
    id.type = OFFHAND;
    id.color = COLOR_MAGENTA;
    id.weight = Die(1,0,1);
    id.hitBonus = Die(0,0,1);
    id.damageBonus = Die(0,0,1);
    id.specialAttribute = Die(0,0,1);
    id.value = Die(9,1,6);
    id.dodgeBonus = Die(10,0,1);
    id.description="A very high-quality shield. Nobody knows what it looks like, except for \
maybe--now--you, but you're not telling. The back side is inscribed with the \
words: \"If found, please return to Zeus, Mount Olympus\".";
    _itemReg.registry.push_back(id);
    id=ItemDescription();

}