#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <fstream>

#include <ncurses.h>

#include "fileIO.h"
#include "defs.h"
#include "algo.h"

MonsterRegistry _monsterReg;

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

        /*
        printf("Load Room %d : (%d, %d) [%d x %d]\r\n", 
            i+1, 
            buf[i][1], buf[i][0],
            buf[i][3], buf[i][2]);*/
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

    //printf("Headers saved\r\nWriting hardness\r\n");

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

            /*
            printf("Save Room %d : (%d, %d) [%d x %d]\r\n", 
                i+1, 
                d->roomInfo[i][1], d->roomInfo[i][0],
                d->roomInfo[i][3], d->roomInfo[i][2]);*/
        }
    }

    //write rooms

    free(savePath);
    fflush(f);
    fclose(f);
}

Die getDie(string raw){
    Die d;

    string tmp;

    for (char c : raw){
        if (c == '+'){
            d.offset = atoi(tmp.c_str());
            tmp = "";
        } else if (c == 'd'){
            d.rolls = atoi(tmp.c_str());
            tmp = "";
        } else {
            tmp += c;
        }
    }

    d.range = atoi(tmp.c_str());

    return d;
}

vector <string> splitBy(string str, char del){
    vector <string> ret;

    size_t index = 0;

    for (size_t x = 0; x <= str.length(); ++x){
        if ((str[x] == del || (x+1) == str.length()) && index < x){
            string cpy = str.substr(index, (x-index) + ((x+1) == str.length()));
            index = x + 1;
            ret.push_back(cpy);
        }
    }

    return ret;
}

string getDatString(string fterm, string raw){
    raw.erase(raw.begin(), raw.begin()+fterm.size());
    return raw;
}

//Monsters
void loadProfiles(vector <string> lines){

    Entity ret = Entity(0,0,0);

    enum offset {_name, _desc, _color, _speed, _abil, _hp, _dmg, _rare};
    int inst [8] = {0};
    
    bool begin = false;

    MonsterDefinition def;

    for (int i = 0; i < lines.size(); ++i){
        string str = lines[i];
        if (str[str.size()-1] == '\r')
            str.pop_back();
        vector <string> terms = splitBy(str, ' ');

        if (terms.size() == 0)
            continue;

        if (begin == false){
            if (terms.size() != 2)
                continue;
            
            begin = (terms[0] == "BEGIN" && terms[1] == "MONSTER");
            if (begin)
                def = MonsterDefinition();

            continue;
        } else {
            if (terms[0] == "END"){
                begin = false;
                _monsterReg.registry.push_back(def);
                continue;
            }
        }

        if (terms[0] == "NAME"){
            def.name = getDatString(terms[0], str);
            inst[_name]++;

            printf("NAME = %s\n", def.name.c_str());
        } else if (terms[0] == "DESC"){
            inst[_desc]++;
            for (++i;i < lines.size() ;++i){
                if (lines[i][0] == '.')
                    break;
                def.description += lines[i] + "\n";
            }

            printf("DESC = %s\n", def.description.c_str());
        } else if (terms[0] == "COLOR"){

            inst[_color]++;

            for (int j = 1; j < terms.size(); ++j){
                string clr = terms[j];

                if (clr == "BLACK") def.colors.push_back(COLOR_WHITE);
                else if (clr == "RED") def.colors.push_back(COLOR_RED);
                else if (clr == "GREEN") def.colors.push_back(COLOR_GREEN);
                else if (clr == "YELLOW") def.colors.push_back(COLOR_YELLOW);
                else if (clr == "BLUE") def.colors.push_back(COLOR_BLUE);
                else if (clr == "MAGENTA") def.colors.push_back(COLOR_MAGENTA);
                else if (clr == "CYAN") def.colors.push_back(COLOR_CYAN);
                else if (clr == "WHITE") def.colors.push_back(COLOR_WHITE);

            }
        } else if (terms[0] == "SPEED"){
            inst[_speed]++;
            Die d = getDie(terms[1]);
            def.speed = d;
            printf("SPEED = %d + %dd%d\n", 
                def.speed.offset,
                def.speed.rolls,
                def.speed.range);
        } else if (terms[0] == "ABIL"){
            inst[_abil]++;
            //def._abil = getDie(terms[1]);
            /*printf("ABIL = %d + %dd%d\n", 
                def.speed.offset,
                def.speed.rolls,
                def.speed.range);*/
        } else if (terms[0] == "HP"){
            inst[_hp]++;
            def.health = getDie(terms[1]);
            printf("HP = %d + %dd%d\n", 
                def.health.offset,
                def.health.rolls,
                def.health.range);
        } else if (terms[0] == "DAM"){
            inst[_dmg]++;
            def.attack = getDie(terms[1]);
            printf("DAM = %d + %dd%d\n", 
                def.attack.offset,
                def.attack.rolls,
                def.attack.range);
        } else if (terms[0] == "RRTY"){
            inst[_rare]++;
            def.rarity = atoi(terms[1].c_str());
            printf("RRTY = %d\n", def.rarity);
        } else {

        }
    }
}

void loadMonsterDefs(){
    string path = getGameDir();
    path += "monster_desc.txt";

    ifstream i(path);
    if (!i){
        printf("Monster file not found at %s\r\n", path.c_str());
        return;
    }

    //state checks
    bool hasMeta = false;
    bool readMonster = false;
    bool nextMonster = false;

    vector <string> lines;

    for(string line; getline(i, line);){
        lines.push_back(line);
    }

    loadProfiles(lines);

    i.close();
} 