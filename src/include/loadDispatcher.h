
#ifndef TEST_LOADDISPATCHER_H
#define TEST_LOADDISPATCHER_H
#include "slaveEngine.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
/*
    Estructura la cual va a manejar la comunicacion con cada uno de los procesos esclavos
*/
#define MAXBUFFER 250
/*
    Struct el cual guarda la informacion para el manejo de los slaves
*/
typedef  struct SlaveManager {
    slaves * pipes;
    int filesCount;
    int filesDone;
    int slaveCount;
    int lastView;
    fd_set * fdset;
    int inSet; 
}slavesManager;

/*
    Funcion la cual te va a devolver el procesamiento de algun archivo si es que hay alguno
    @params: manager-> estructura que tiene la informacion sobre los fd y todo.
            buffer -> buffer en donde se pondra la informacion del archivo.

*/
void getDone(slavesManager * manager,char * buffer);

#endif
