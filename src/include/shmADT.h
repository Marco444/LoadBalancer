#ifndef SHM_ADT_H
#define SHM_ADT_H

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

typedef struct shmCDT * shmADT;

/*
*   Creates a shared memory, and returns a handler for the shared memory and synchronization between processes
*/
shmADT createSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot);


/*
*   Opens a shared memory, that is already created
*   and returns a handler for the shared memory and synchronization between processes
*/
shmADT openSHM(const char * shm_name, const char * sem_name, int oflags, mode_t mode, unsigned int shmSize, int prot);
/*
*   Closes and frees the shared memory and sync resources.
*   shmAdt -> handler
*/
void closeSHM(shmADT shmAdt);

/*
*   Writes into the shared memory. It will use semphores in order to sync with reader.
*/
void writeSHM(shmADT shmAdt, char * buffer);

/*
*   Reads the shared memory. It will use semphores in order to sync with writer.
*/
void readSHM(shmADT shmAdt, char * buffer);

#endif 