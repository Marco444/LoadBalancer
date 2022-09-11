//
// Created by Marco Scilipoti on 05/09/2022.
//
#ifndef slave_Engine_H
#define slave_Engine_H



/*
 * Esta parte viene a encargarse de distribuir las tasks entre los procesos, notar que una vez formado los
 * batches de trabajo todavia nos queda distribuirlos entre los procesos que se van a encargar de hacerlos.
 *
 * Esencialemnte las limitaciones al hacer el dispatcher son dos: primero que puedo tener mas loads que
 * esclavos para enviarles mi archivo y segundo que la distribucion de loads esta hecha de manera aproximada
 * (el problema es NP), por lo tanto puede ser que queden procesos esperando a otros terminar mas de lo que se\
 * deberia.
 *
 * El dispatcher entonces ademas de manejar las loads para un unico proceso, se encargaria de tner una seccion de loads,
 * una pool, donde se guardarian las loads de un un grupo de procesos/
 * */



////////////////////////////////////////////////////////////////////////////
//como mando una string dinamica como argumento de execv y dsp la libero????
// Puede ser que no lo necesite, se que un fd tiene un tamanio de un entero!
// Lo mismo puede pensarse en usar lo de la longitud de los nombres de los archivos
///////////////////////////////////////////////////////////////////////////
#include "lib.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
typedef struct slaves{
    int * readFD;
    int * writeFD;
} slaves;
typedef  struct slaveManager {
    slaves * pipes;
    int filesCount;
    int filesDone;
    int slaveCount;
    int lastView;
    fd_set * fdset;
    int inSet; 
} * SlavesManager;
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
/*
    Fucion la cual crea el manager el cual manejara a todos los slaves
    @paramas: int slavesCount -> cantiad de esclavos que queres crear
                int totalTask -> total de los archivos a procesar
*/
SlavesManager createManager(int slavesCount, int totalTask);
/*
    Funcion la cual se encarga de liberar toda la memoria utilizada por los esclavos
    y cerrar los fds
*/
void freeManager(SlavesManager toFree);
/*
    Funcion la cual se encarga de devuelve la cantidad de files hechas
*/
int getDoneFile(SlavesManager manager);
/*
    Funcion la cual devuelve el ultimo archivo visto
*/
int getLastView(SlavesManager manager);
#endif
