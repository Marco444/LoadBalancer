// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../include/lib.h"
#include "../../include/shmADT.h"
#include <stdio.h>

int main(int argc, char * argv[]){

    int filesCant;
    //TODO check errors here?
    if(argc > 1){
        sscanf(argv[1], "%d", &filesCant);
    }else{
        scanf("%d", &filesCant);
    }

    unsigned int size = MAXBUFFER * filesCant;
    shmADT shm = openSHM(SHM_NAME, SEM_NAME, O_RDWR, 0, size, PROT_READ);

    char buffer[MAXBUFFER];

    for(int i = 0; i < filesCant; i++){
        readSHM(shm, buffer);
        printf("%s\n", buffer);
    }

    closeSHM(shm);

}