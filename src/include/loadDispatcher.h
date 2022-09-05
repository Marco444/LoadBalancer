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
///////////////////////////////////////////////////////////////////////////

typedef struct slaves{
    int * readFD;
    int * writeFD;
    int * slavesIds;
} slaves;



void createChild(char * file, int read_addr, int write_addr);


slaves * createSlaves(int slaveCount);

void secureFreeSlave(slaves * freeElement)

#endif //TEST_LOADDISPATCHER_H
