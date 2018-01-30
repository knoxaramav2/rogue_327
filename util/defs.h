#ifndef DEFS
#define DEFS

//size
#define DUNGEON_HEIGHT  21
#define DUNGEON_WIDTH   80
#define MIN_ROOM        5
#define MAX_ROOM        12
#define MIN_ROOM_SIZE   3
#define MAX_ROOM_SIZE   15

//legend
#define OPEN_SPACE      '.'
#define ROCK_SPACE      ' '
#define HALL_SPACE      '#'

//bit masks
#define SYMBOL_MASK     0x0000FFFF
#define HARD_MASK       0x00070000
#define IMMUTABLE_MASK  0x00080000

#define HARD_OFFSET     16
#define MUTABLE_OFFSET  19

#endif