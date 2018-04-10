#include <string.h>
#include <stdlib.h>
//#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <ncurses.h>

#include "defs.h"
#include "dungeon.h"
#include "console.h"
#include "config.h"
#include "fileIO.h"
#include "algo.h"
#include "game.h"

Config config;
Console __console;

void printHelp(){

    clear();

    mvprintw(0, 0, "--HELP--");

    mvprintw(1, 0, "-h       Print this text");
    mvprintw(2, 0, "--save   Enable saving");
    mvprintw(3, 0, "-s       Enable saving");
    mvprintw(4, 0, "--load   Load game");
    mvprintw(5, 0, "-l       Load game");
    mvprintw(6, 0, "--nummon [integer]");

    refresh();
}

void printCmdErr(int err, char * msg){

    clear();

    static char messages [8][128] = {
        "Switches must start with one to two '-''s\0", 
        "Unrecognized option\0",
        "Parameter must be an integer"
    };

    mvprintw(0, 0, "(Err %d) %s [%s]", err, messages[err], msg);

    refresh();
}

//read command line arguments
void cmd(int argc, char ** argv){

    config._run = 1;

    //defaults
    config.load = 0;
    config.save = 0;

    int i = 1;
    for (; i < argc; ++i){
        char * op = argv[i];

        if (strlen(op) == 1 || op[0] != '-'){
            printCmdErr(0, op);
            config._run = 0;
            continue;
        }

        //full options
        if (op[1] == '-'){
            if (strncmp(op, "--save", 255) == 0)
                config.save = 1;
            else if (strncmp(op, "--load", 255) == 0)
                config.load = 1;
            else if (strncmp(op, "--nummon", 255) == 0){
                if (++i == argc){
                    printCmdErr(2, argv[i]);
                    config._run = 0;
                }

                config.numNpc = atoi(argv[i]);
            } else {
                printCmdErr(1, op);
                config._run = 0;
            }
        } else {//aggregate options
            int j = 1;
            for (; j < strlen(op); ++j){
                switch(op[j]){
                    case 'h': printHelp(); break;
                    case 's': config.save = 1;break;
                    case 'l': config.load = 1;break;
                    default:
                        printCmdErr(1, op);
                        config._run = 0;
                }
            }
        }
    }
}

//UTIL
int main(int argc, char ** argv){

    #ifdef __CYGWIN
    //printf("CYGWIN DEPENDANT\r\n");
    //fflush(stdout);
    #endif

    //setup
    cmd(argc, argv);
    srand(time(0));

    if (config._run == 0){
        mvprintw(0, 0, "Argument errors found; quitting");
        //printf("Argument errors found; quitting\r\n");
        endwin();
        return -1;
    }


    
    Dungeon * dungeon = 0;

    loadMonsterDefs();
    //return 0;

    //determine load/save/new=
    if (config.load){
        dungeon = loadGame();
    } else {
        dungeon = generateDungeon(NULL);
    }

    if (dungeon == 0){
        //printf("Unable to get dungeon\r\n");
        return -1;
    }

    //play game
    setupGameState(dungeon);
    spawnPlayers(dungeon);

    calcDistMap(dungeon, 0);

    //renderDistance(dungeon, 0);
    //renderDistance(dungeon, 1);
    //renderScreen(dungeon);

    updateScreen(dungeon);

    while (config._run){
        updateTurn(&dungeon);
    }

    updateScreen(dungeon);

    endwin();

    //printf("Game over\r\n");
    
    if (config.save){
        saveGame(dungeon);
    }

    return 0;
}