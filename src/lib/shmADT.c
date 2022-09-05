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
    char * address;
    int pos;
    int sizeSHM;
    sem_t * sem;
    int creator;
    
}shmCDT;



shmADT createSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot){

    shmADT shmAdt = calloc(1, sizeof(shmCDT)); 

    if(shmAdt == NULL){ 
        perror("Error while initializing Shared Memory");
        exit();
    }

    shmAdt->nameSHM = shm_name;
    shmAdt->sizeSHM = shmSize;

    int shm_fd = shm_open(shm_name, oflags, mode);

    if(shm_fd == -1){
        perror("Error while initializing Shared Memory");    
        exit();
    }

    // We establish the size and check for errors
    if(ftruncate(shm_fd, shmSize) == -1){
        perror("Error while initializing Shared Memory");
        exit();
    }

    shmAdt->address = mmap(NULL, shmSize, prot, MAP_SHARED, shm_fd, 0);
    shmAdt->pos = 0;

    if(shmAdt->address == MAP_FAILED){
        perror("Error while initializing Shared Memory");
        exit();
    }


    if(close(shm_fd)==-1){
        //unlinkShm(shmv);
        //freeShm(shm);
        perror("Error closing file descriptor");
        exit();
    }

    
    // We create the samphore
    shmAdt->sem = sem_open(sem_name, oflags, mode, 0);

    if(shmAdt->sem == SEM_FAILED){
        //unlinkShm(shm);
        //freeShm(shm);
        perror("Error creating semaphore");
        exit();
    }
    shmAdt->creator = 1;

    return shmAdt;
}

shmADT openSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot){

    shmADT shmAdt = calloc(1, sizeof(shmCDT)); 

    if(shmAdt == NULL){ 
        perror("Error while initializing Shared Memory");
        exit();
    }

    shmAdt->nameSHM = shm_name;
    shmAdt->sizeSHM = shmSize;

    int shm_fd = shm_open(shm_name, oflags, mode);

    if(shm_fd == -1){
        perror("Error while initializing Shared Memory");    
        exit();
    }

    shmAdt->address = mmap(NULL, shmSize, prot, MAP_SHARED, shm_fd, 0);
    shmAdt->pos = 0;

    if(shmAdt->address == MAP_FAILED){
        perror("Error while initializing Shared Memory");
        exit();
    }


    if(close(shm_fd)==-1){
        //unlinkShm(shm);
        //freeShm(shm);
        perror("Error closing file descriptor");
        exit();
    }

    
    // We open the samphore
    shmAdt->sem = sem_open(sem_name, 0);
    if(shmAdt->sem == SEM_FAILED){
        //unlinkShm(shm);
        //freeShm(shm);
        perror("Error opening semaphore");
        exit();
    }
    shmAdt->creator = 0;

    return shmAdt;
}


void closeSHM(shmADT shmAdt){
    if(NULL == shmAdt)
        perror("Invalid parameters for closeSHM");

    if(-1 == munmap(shmAdt->address, shmAdt->sizeSHM)){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
        free(shmAdt);
        perrorExit("Error unmapping address");
    }
    
    if(-1 == sem_close(shmAdt->sem)){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
        free(shmAdt);
        perrorExit("Error closing sem");
    }

    if(shmAdt->creator){
        unlinkSem(shmAdt);
        unlinkShm(shmAdt);
    }

    freeShm(shmAdt);
}
   
void writeSHM(shmADT shmAdt, char * buffer){

    if(shmAdt == NULL || buffer == NULL){
        perror("Invalid parameters for writeSHM");
    }

    

    for(int i = 0; buffer[i] != '\0'; i++, (shmAdt->pos)++){
        
        if(shmAdt->pos >= shmAdt->sizeSHM)
            perror("Error Shared Memory capacity exceded");

        (shmAdt->address)[shmAdt->pos] = buffer[i];
    }

}


void readSHM(shmADT shmAdt, char * buffer){

}



//________________Static functions________________

static void unlinkSem(shmADT shmAdt){
    if(sem_unlink(shmAdt->nameSEM) == -1){
        perror("Error unlinking Semaphore");
        exit();
    }
}

static void unlinkShm(shmADT shmAdt){
    if(shm_unlink(shmAdt->nameSHM)){
        perror("Error unlinking Shared Memory");
        exit();
    }
}