#ifndef DEFS
#define DEFS

//size
#define DUNGEON_HEIGHT  21
#define DUNGEON_WIDTH   80
#define MIN_ROOM        5
#define MAX_ROOM        12

//legend
#define OPEN_SPACE      '.'
#define ROCK_SPACE      ' '
#define HALL_SPACE      '#'

//bit masks
#define SYMBOL_MASK     0x0000FFFF
#define HARD_MASK       0x000F0000
#define MUTABLE_MASK    0x00F00000

#endif