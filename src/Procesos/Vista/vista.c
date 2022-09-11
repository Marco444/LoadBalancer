// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../../include/lib.h"
#include "../../include/shmADT.h"
#include <stdio.h>

int main(int argc, char * argv[]){

    int filesCant;
    
    //In case we recieve more than 1 parameter we asume that argv[1] will cointain the amount of files hashed with md5
    if(argc > 1){
        if(sscanf(argv[1], "%d", &filesCant) == -1){
            perror("Invalid parameter recieved : ");
            exit(errno);
        }
    }
    //Otherwise, we will recieve via stdin the amount of files hashed with md5
    else{
        if(scanf("%d", &filesCant) == -1){
            perror("Invalid parameter recieved : ");
            exit(errno);
        }
    }

    // Checks if an invalid value has been recieved
    if(filesCant < 0){
        perror("Invalid value for parameter : ");
        exit(1);
    }

    // The Shared Memory Size will be defined by the amount of files times the MAXBUFFER size
    unsigned int size = MAXBUFFER * filesCant;
    // The Shared Memory is opened by the ADT
    shmADT shm = openSHM(SHM_NAME, SEM_NAME, O_RDWR, 0, size, PROT_READ);

    // 'buffer' will be used for printing each file.
    char buffer[MAXBUFFER];

    // We iterate between each file, and read the md5 hashing output by APLICATION
    for(int i = 0; i < filesCant; i++){
        // We read the Shared Memory via the ADT
        readSHM(shm, buffer);
        // We print the output
        printf("%s", buffer);
    }

    // We close al the Shared Memory resources via the ADT
    closeSHM(shm);

    return 0;

}