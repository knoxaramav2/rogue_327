#ifndef CONFIG
#define CONFIG

class Config {

    public:
    int load;
    int save;

    int _run;

    int numNpc;
};

extern Config config;

#endif