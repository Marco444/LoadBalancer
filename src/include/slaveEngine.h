
#ifndef slave_Engine_H
#define slave_Engine_H
#include "loadDispatcher.h"


typedef struct {
    sComunication * pipes;
    int filesCount;
    int filesDone;
}slavesManager;



#endif
