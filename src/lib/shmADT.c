#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>

//TODO check if there are unnecesary #include's

#include "shmADT.h"

typedef struct shmCDT{

    const char * nameSHM;
    const char * nameSEM;
    sem_t sem;
    int sizeSHM;
    
}shmCDT;



shmADT openSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot){

    shmADT shm = calloc(1, sizeof(shmCDT)); 

    if(shm == NULL)
        perror("Heap error while initializing Shared Memory");

    shm->nameSHM = shm_name;
    shm->sizeSHM = shmSize;

}


void closeSHM(shmADT shmAdt){

}


void writeSHM(shmADT shmAdt, char * buffer){

}


void readSHM(shmADT shmAdt, char * buffer){

}