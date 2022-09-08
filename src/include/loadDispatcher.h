
#ifndef LOAD_DISPATCHER_H
#define LOAD_DISPATCHER_H

#include "slavesEngine.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
/*
    Estructura la cual va a manejar la comunicacion con cada uno de los procesos esclavos
*/
 struct slaveManager;
 typedef SlavesManager * slaveManager ;


/*typedef struct slaves{
    int * readFD;
    int * writeFD;
} slaves;
/*
    Funcion la cual crea a un hijo con sus respectivos pipes
    @params: file -> aplicacion a la cual le hara el execv
            read_addr -> file descriptor en donde leera el hijo
            write_addr-> el file descriptor en donde escribira el hijo
*/
void createChild(char * file, int read_addr, int write_addr);
/*
    Funcion la cual crea a los esclavos con sus respectivos pipe
    @params: slaveCount -> cantidad de esclavos que se quieren crear
    @returnValue: Un struct en donde se encuentra los fd de lectura a todos los hijos
    ,los fd de escritura a todos los hijos y los pid de cada uno. 
*/
slaves * createSlaves(int slaveCount);
/*
    Funcion la cual se encarga de hacer un free de la estructura slaves
    @params: freeElemet -> elemento al cual le queremos hacer un free
*/
void secureFreeSlave(slaves *freeElement,int cant);

SlavesManager createManager(int slavesCount, int totalTask);
/*
    Funcion la cual te va a devolver el procesamiento de algun archivo si es que hay alguno
    @params: manager-> estructura que tiene la informacion sobre los fd y todo.
            buffer -> buffer en donde se pondra la informacion del archivo.

*/
void readSlave(SlavesManager manager,char * buffer);
void writeSlave(SlavesManager manager, char *file, int slaveNum);

#endif
