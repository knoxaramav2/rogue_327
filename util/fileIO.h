#ifndef FILE_IO
#define FILE_IO

    #include <string>

    #include "dungeon.h"
    #include "entity.h"

    Dungeon * loadGame();
    void saveGame(Dungeon *);
    void loadDefinitions(std::string);

#endif