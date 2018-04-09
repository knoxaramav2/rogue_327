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
#define UP_STAIRS       '<'
#define DOWN_STAIRS     '>'

//bit masks
#define SYMBOL_MASK     0x0000FFFF
#define HARD_MASK       0x00FF0000
#define IMMUTABLE_MASK  0x00FF0000

#define HARD_OFFSET     16
#define MUTABLE_OFFSET  16


//key values
#define BOTTOM_LEFT     0
#define BOTTOM_MIDDLE   1
#define BOTTOM_RIGHT    2
#define MIDDLE_LEFT     3
#define MIDDLE_RIGHT    4
#define TOP_LEFT        5
#define TOP_MIDDLE      6
#define TOP_RIGHT       7

#define MOVE_LOWER      10
#define MOVE_UPPER      11
#define REST_TURN       12

#define LIST_MONSTERS   20

#define SCROLL_UP       30
#define SCROLL_DOWN     31

#define ESCAPE          40

#define QUIT_GAME       50

#define FOG_OF_WAR      60
#define TELEPORT        61

#define INVALID_KEY     -1

#endif