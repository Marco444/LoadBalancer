// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by Marco Scilipoti on 05/09/2022.
//

#include "../../include/loadDispatcher.h"
#include "../../include/lib.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void createChild(char *file, int read_addr, int write_addr)
{

    // seteo de la entrada y salida entrada del proyecto esclavo
    close(0);
    close(1);
    dup2(read_addr, 0);
    dup2(write_addr, 1);
    if (close(read_addr) == -1 || close(write_addr) == -1)
        perror("Error closing pipes");

    execv(file, (char **){NULL});
}

slaves *createSlaves(int slaveCount)
{
    // TODO habria que ver si se podria cambiar de lugar esto asi no creo inecesariamente con malloc
    slaves *slavesPipes = malloc_c(sizeof(slaves));
    slavesPipes->readFD = malloc_c(sizeof(int) * slaveCount);
    slavesPipes->writeFD = malloc_c(sizeof(int) * slaveCount);
    for (int i = 0; i < slaveCount; ++i)
    {

        // definimos el channelA para que el padre escriba y el hijo lea
        int channelA[2];
        // definimos le channelB para que el padre lea y el hijo escriba
        int channelB[2];

        if (pipe(channelA) == -1 || pipe(channelB) == -1)
        {
            secureFreeSlave(slavesPipes,i);
            perror("Creation pipe error");
            exit(1);
        }


        if ( fork() == 0)
        {

            if (close(channelA[1]) == -1 || close(channelB[0]) == -1)
                perror("Error closing pipes");
            // En este for lo que hacemos es cerrar los fds creados por los demas slaves
            for (int j = 0; j < i; j++)
            {
                close(slavesPipes->readFD[j]);
                close(slavesPipes->writeFD[j]);
            }
            // Guardo los pipes para que luego poder hacer el waite cuando terminen
            createChild("ESCLAVO", channelA[0], channelB[1]);
        }
        else
        {
            if (close(channelA[0]) == -1 || close(channelB[1]) == -1)
                perror("Error closing pipes");
            
            slavesPipes->readFD[i] = channelB[0];
            slavesPipes->writeFD[i] = channelA[1];
        }
    }
    return slavesPipes;
}

SlavesManager createManager(int slavesCount, int totalTask){
    fd_set * fdSet =malloc_c(sizeof(fd_set));
    FD_ZERO(fdSet);
    SlavesManager manager = malloc_c(sizeof(struct slaveManager));
    manager->fdset = fdSet;
    manager->filesCount = totalTask;
    manager->filesDone = 0;
    manager ->inSet = 0;
    manager ->lastView = 0;
    manager ->pipes = createSlaves(slavesCount);
    manager->slaveCount = slavesCount;
    return manager;    

}
void secureFreeSlave(slaves *freeElement,int cant)
{
    for(int i = 0; i < cant;i++){
        close(freeElement->readFD[i]);
        close(freeElement->writeFD[i]);
    }
    free(freeElement->readFD);
    free(freeElement->writeFD);
    free(freeElement);
}
void freeManager(SlavesManager toFree){
    secureFreeSlave(toFree->pipes,toFree->slaveCount);
    free(toFree->fdset);
    free(toFree);
}
int getDoneFile(SlavesManager manager){
    return manager->filesDone;
}
int getLastView(SlavesManager manager){
    return manager->lastView;
}
