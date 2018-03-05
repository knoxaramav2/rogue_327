#ifndef CONFIG
#define CONFIG

typedef struct Config {
    int load;
    int save;

    int _run;

    int numNpc;
} Config;

extern Config config;

#endif