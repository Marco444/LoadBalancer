// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/lib.h"

//TODO check if there are unnecesary #include's

#include "../../include/shmADT.h"

static void unlinkSem(shmADT shmAdt);
static void unlinkShm(shmADT shmAdt);

typedef struct shmCDT{

    const char * nameSHM;
    const char * nameSEM;
    char * address;
    int writePos;
    int readPos;
    int sizeSHM;
    sem_t * sem;
    int creator;
    
}shmCDT;

shmADT createSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot){

    shmADT shmAdt = malloc_c(sizeof(shmCDT)); 

    if(shmAdt == NULL){ 
        perror("Error while initializing Shared Memory");
        exit(errno);
    }

    shmAdt->nameSHM = shm_name;
    shmAdt->sizeSHM = shmSize;

    int shm_fd = shm_open(shm_name, oflags, mode);

    if(shm_fd == -1){
        perror("Error while initializing Shared Memory");    
        exit(errno);
    }

    // We establish the size and check for errors
    if(ftruncate(shm_fd, shmSize) == -1){
        perror("Error while initializing Shared Memory");
        exit(errno);
    }

    shmAdt->address = mmap(NULL, shmSize, prot, MAP_SHARED, shm_fd, 0);
    shmAdt->readPos = 0;
    shmAdt->writePos = 0;

    if(shmAdt->address == MAP_FAILED){
        perror("Error while initializing Shared Memory");
        exit(errno);
    }


    if(close(shm_fd)==-1){
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while closing file descriptor");
        exit(errno);
    }
    
    // We create the samphore
    shmAdt->sem = sem_open(sem_name, oflags, mode, 0);
    shmAdt->nameSEM = sem_name;

    if(shmAdt->sem == SEM_FAILED){
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while creating semaphore");
        exit(errno);
    }
    shmAdt->creator = 1;

    return shmAdt;
}


shmADT openSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot){

    shmADT shmAdt = malloc_c(sizeof(shmCDT)); 

    if(shmAdt == NULL){ 
        perror("Error while initializing Shared Memory (1)");
        exit(errno);
    }

    shmAdt->nameSHM = shm_name;
    shmAdt->sizeSHM = shmSize;

    int shm_fd = shm_open(shm_name, oflags, mode);

    if(shm_fd == -1){
        perror("Error while initializing Shared Memory (2)");    
        exit(errno);
    }

    shmAdt->address = mmap(NULL, shmSize, prot, MAP_SHARED, shm_fd, 0);
    shmAdt->readPos = 0;
    shmAdt->writePos = 0;

    if(shmAdt->address == MAP_FAILED){
        perror("Error while initializing Shared Memory (3)");
        exit(errno);
    }


    if(close(shm_fd)==-1){
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while closing file descriptor");
        exit(errno);
    }

    
    // We open the samphore
    shmAdt->sem = sem_open(sem_name, 0);
    if(shmAdt->sem == SEM_FAILED){
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while opening semaphore");
        exit(errno);
    }
    shmAdt->creator = 0;

    return shmAdt;
}


void closeSHM(shmADT shmAdt){
    if(shmAdt == NULL){
        perror("Invalid parameters for closeSHM");
        exit(errno);
    }

    if(munmap(shmAdt->address, shmAdt->sizeSHM) == -1){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while unmapping address");
        exit(errno);
    }
    
    if(sem_close(shmAdt->sem) == -1){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
        free(shmAdt);
        perror("Error while closing sem");
        exit(errno);
    }

    if(shmAdt->creator){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
    }

    free(shmAdt);
}
   

void writeSHM(shmADT shmAdt, char * buffer){

    if(shmAdt == NULL || buffer == NULL){
        perror("Invalid parameters for writeSHM");
        exit(errno);
    }

    for(int i = 0; buffer[i] != '\n'; i++, (shmAdt->writePos)++){
        
        if(shmAdt->writePos >= shmAdt->sizeSHM){
            perror("Error Shared Memory capacity exceded");
            return;
            }
        

        (shmAdt->address)[shmAdt->writePos] = buffer[i];
    }

    (shmAdt->address)[(shmAdt->writePos)++] = '\n';

//    (shmAdt->address)[(shmAdt->writePos)] = '\0';

    // sem post!
    if(sem_post(shmAdt->sem) == -1){
        closeSHM(shmAdt);
        perror("Error while writing in Shared Memory");
        exit(errno);
    }

}


void readSHM(shmADT shmAdt, char * buffer){

    if(shmAdt == NULL || buffer == NULL){
        perror("Invalid parameters for writeSHM");
        exit(errno);
    }

    //sem wait!
    if(sem_wait(shmAdt->sem) == -1){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
        free(shmAdt);
        exit(errno);
    }

    int i = 0;
    for(; (shmAdt->address)[shmAdt->readPos] != '\n'; i++, (shmAdt->readPos)++){
        buffer[i] = (shmAdt->address)[shmAdt->readPos];
    }
    //buffer[++i] = '\n';
    buffer[i++] = '\n';
    buffer[i++] = '\0';
    (shmAdt->readPos)++;
    //(shmAdt->readPos)++;
}


//________________Static functions________________


static void unlinkSem(shmADT shmAdt){
    if(sem_unlink(shmAdt->nameSEM) == -1){
        perror("Error unlinking Semaphore");
        exit(errno);
    }
}


static void unlinkShm(shmADT shmAdt){
    if(shm_unlink(shmAdt->nameSHM) == -1){
        perror("Error unlinking Shared Memory");
        exit(errno);
    }
}