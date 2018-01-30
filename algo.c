#include <stdlib.h>

#include "algo.h"

int randIn(int min, int max){
    return (rand()%(max-min)) + min;
}