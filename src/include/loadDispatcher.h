//
// Created by Marco Scilipoti on 05/09/2022.
//

#ifndef TEST_LOADDISPATCHER_H
#define TEST_LOADDISPATCHER_H


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
#include <lib.h>
typedef struct slaves{
    int * readFD;
    int * writeFD;
    int * slavesIds;
    int * countLoads;
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
void secureFreeSlave(slaves * freeElement);

#endif
