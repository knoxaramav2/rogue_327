#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include "defs.h"
#include "dungeon.h"
#include "console.h"
#include "config.h"
#include "fileIO.h"

Config config;

void printHelp(){
    printf("---HELP---\r\n");

    printf("-h      Print this text\r\n");
    printf("--save  Enable saving\r\n");
    printf("-s      Enable saving\r\n");
    printf("--load  Load game\r\n");
    printf("-l      Load game\r\n");
    printf("\n\n\r");
}

void printCmdErr(int err, char * msg){
    static char messages [8][128] = {
        "Switches must start with one to two '-''s\0", 
        "Unrecognized option\0"
    };

    printf("(Err %d) %s\r\n", err, messages[err]);
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
            if (strncmp(op, "--save", 0) == 0)
                config.save = 1;
            else if (strncmp(op, "--load", 0) == 0)
                config.load = 1;
            else {
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

    //setup
    cmd(argc, argv);
    srand(time(0));

    if (config._run == 0){
        printf("Argument errors found; quitting\r\n");
        return -1;
    }
    
    Dungeon * dungeon = 0;

    //determine load/save/new=
    if (config.load){
        dungeon = loadGame();
    } else {
        dungeon = generateDungeon();
        if (dungeon == 0){
            printf("Unable to generate dungeon\r\n");
        return -1;
        }
    }

    //play game

    if (config.save){
        saveGame(dungeon);
    }

    



    renderScreen(dungeon);

    return 0;
}