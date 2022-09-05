
#ifndef slave_Engine_H
#define slave_Engine_H
#include "loadDispatcher.h"

/*
    Estructura la cual va a manejar la comunicacion con cada uno de los procesos esclavos
*/
typedef struct {
    slaves * pipes;
    int filesCount;
    int filesDone;
}slavesManager;



#endif
