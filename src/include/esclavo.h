#ifndef esclavo_H
#define esclavo_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#define MAXBUFFER 250

/*
    Params: buffer en donde se pondra el valor del md5 con el nombre de aplicacion
    void md5Calculate(char * buffer);
*/
void md5Calculate(char * buffer);





#endif