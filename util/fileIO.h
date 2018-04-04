#ifndef FILE_IO
#define FILE_IO

    #include "dungeon.h"
    #include "entity.h"

    Dungeon * loadGame();
    void saveGame(Dungeon *);
    void loadMonsterDefs();

#endif