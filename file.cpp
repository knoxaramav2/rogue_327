#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/stat.h>

#include "fileIO.h"
#include "defs.h"
#include "algo.h"

using namespace std;

unsigned long calculateFileSize(Dungeon * d){
    return 1700 + (4 * d->roomCount);
}

char * getGameDir(){

    size_t hLen, gLen;

    char * home = getenv("HOME");
    char gfolder [] = "/.rlg327/";

    hLen = strlen(home);
    gLen = strlen(gfolder);

    char * ret = (char *) malloc(hLen + gLen + 1);
    strncpy(ret, home, hLen);
    strncpy(ret+hLen, gfolder, gLen + 1);

    return ret;
}

char * getSaveDir(){
    
    size_t gLen, sLen;

    char * gdir = getGameDir();
    char sdir [] = "dungeon";
    
    gLen = strlen(gdir);
    sLen = strlen(sdir);

    char * ret = (char *) malloc(gLen + sLen + 1);
    strncpy(ret, gdir, gLen);
    strncpy(ret+gLen, sdir, sLen+1);

    free(gdir);

    return ret;

}

void ensureDir(){
    char * gameDir = getGameDir();
    mkdir(gameDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    free(gameDir);
}

unsigned char getByte(unsigned long raw, int idx){
    return (raw >> (idx * 8)) && 0xFF;
}

unsigned long swapEndian(unsigned long raw){
    unsigned long ret;
    unsigned long mask = 0xFF;

    ret |= (raw >> 24) & mask;
    ret |= (raw >> 16) & mask << 8;
    ret |= (raw >> 8) & mask << 16;
    ret |= (raw) & mask & mask << 24;

    return ret;
}

void writeStream(string stream, size_t len, FILE * f){
    size_t i = 0;
    for (; i < len; ++i){
        fputc(stream[i], f);
    }
}

//return 32 bit unsigned as big endian representative string
unsigned char * ui2str(unsigned i){
    unsigned char * str = (unsigned char *) malloc(4);

    str[0] = getByte(i, 3);
    str[1] = getByte(i, 2);
    str[2] = getByte(i, 1);
    str[3] = getByte(i, 0);

    return str;
}

unsigned long int str2ui(char * c){
    unsigned long int i = 0;

    i |= (unsigned long)c << 24;
    i |= (unsigned long)c << 16;
    i |= (unsigned long)c << 8;
    i |= (unsigned long)c;


    return i;
}

Dungeon * loadGame(){

    char * gameDir  = getGameDir();
    char * savePath = getSaveDir();

    mkdir(gameDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    free(gameDir);

    FILE * f = fopen(savePath, "r");

    if (!f){
        return 0;
    }

    free(savePath);

    char cfileType[12];
    char cversion[4];
    char cfileSize[4];

    fread(cfileType, 1, 12, f);
    fread(cversion, 1, 4, f);
    fread(cfileSize, 1, 4, f);

    Dungeon * d = new Dungeon();//createDungeon();

    //load dungeon
    char hardness [DUNGEON_HEIGHT * DUNGEON_WIDTH];
    fread(hardness, 1, DUNGEON_HEIGHT * DUNGEON_WIDTH, f);

    int i = 0;
    for (; i < DUNGEON_HEIGHT * DUNGEON_WIDTH; ++i){    
        unsigned type = hardness[i] == 0 ? HALL_SPACE : ROCK_SPACE;
        d->screen[i] = setHardness(type, hardness[i]);
    }

    //read room data;
    char buf [20][4];
    size_t nRead;
    int rooms = 0;

    while((nRead = fread(buf[rooms], 1, 4, f)) > 0){

        //int l=0;
        //for (; l < 4; ++l)printf("%u %u %u %u", buf[rooms][l]);
        //printf("\r\n");

        ++rooms;
    }

    fflush(stdout);

    //load room data
    d->roomInfo = (int **) malloc(sizeof(int *) * rooms);
    d->roomCount = rooms;

    for (i=0; i < rooms; ++i){
        d->roomInfo[i] = (int *) malloc(sizeof(int) * 4);

        d->roomInfo[i][0] = buf[i][1];
        d->roomInfo[i][1] = buf[i][0];
        d->roomInfo[i][2] = buf[i][3];
        d->roomInfo[i][3] = buf[i][2];

        //populate with floor space
        int x = buf[i][1];
        int y = buf[i][0];
        int sx = buf[i][3];
        int sy = buf[i][2];

        int ix = 0;
        int iy = 0;

        for (ix = x; ix < x+sx; ++ix){
            for (iy = y; iy < y+sy; ++iy){
                d->screen[CELL(ix, iy)] = OPEN_SPACE;
            }
        }

        //d->screen[i] = setHardness(type, hardness[i]);

        printf("Load Room %d : (%d, %d) [%d x %d]\r\n", 
            i+1, 
            buf[i][1], buf[i][0],
            buf[i][3], buf[i][2]);
    }

    //place player in room
    int rId = randIn(0, d->roomCount);
    int * roomSet = d->roomInfo[rId];
    int xLoc = roomSet[0] + (roomSet[2] / 2 );
    int yLoc = roomSet[1] + (roomSet[3] / 2);

    d->player = new Player(xLoc, yLoc);//createEntity('@', xLoc, yLoc, 0);

    return d;
}

void saveGame(Dungeon * d){

    ensureDir();

    char * savePath = getSaveDir();
    FILE * f = fopen(savePath, "w+");
    
    unsigned fileSize = calculateFileSize(d);
    string headerTitle = "RLG327-S2018";
    string empty = "\0\0\0\0";

    //write headers
    writeStream(headerTitle, 12, f);
    writeStream(empty, 4, f);
    writeStream((char *) ui2str(fileSize), 4, f);

    printf("Headers saved\r\nWriting hardness\r\n");

    //write map hardness ;)
    {
        int i=0, j=0;
        for (; i < DUNGEON_HEIGHT * DUNGEON_WIDTH; ++i){
            fputc((unsigned char)getHardness(d->screen[i]), f);
        }

        for(i=0; i<d->roomCount; ++i){
            fputc((unsigned char)d->roomInfo[i][1], f);//y
            fputc((unsigned char)d->roomInfo[i][0], f);//x
            fputc((unsigned char)d->roomInfo[i][3], f);//size y
            fputc((unsigned char)d->roomInfo[i][2], f);//size x

            printf("Save Room %d : (%d, %d) [%d x %d]\r\n", 
                i+1, 
                d->roomInfo[i][1], d->roomInfo[i][0],
                d->roomInfo[i][3], d->roomInfo[i][2]);
        }
    }

    //write rooms

    free(savePath);
    fflush(f);
    fclose(f);
}