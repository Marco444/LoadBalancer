//
// Created by Marco Scilipoti on 05/09/2022.
//

#include "../../include/loadDispatcher.h"
#include "../../include/loadBalancer.h"
#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <stdlib.h>



void createChild(char * file, int read_addr, int write_addr) {

    //define two strings to store parent_child_c of pipe
    char read[20], write[20];
    sprintf(read, "%d", read_addr);
    sprintf(write, "%d", write_addr);   // esto no hace falta

    //define string to hold both fds -> Aca le debemos mandar los primeros archivos que debe desarrollar el slave
    char * fds[] = {read, write};
    char * childArgs[]={file, (char *) fds, NULL};

    execv(file, childArgs);
}

slaves * createSlaves(int slaveCount)
{
    slaves * slavesPipes = malloc(sizeof(slaves));
    slavesPipes -> readFD = malloc(sizeof(int) * slaveCount);
    slavesPipes -> writeFD = malloc(sizeof(int) * slaveCount);
    slavesPipes -> slavesIds = malloc(sizeof(int));
    for (int i = 0; i < slaveCount; ++i) {

        //definimos el channelA para que el padre escriba y el hijo lea
        int channelA[2];
        //definimos le channelB para que el padre lea y el hijo escriba
        int channelB[2];
        if(pipe(channelA) == -1 ||  pipe(channelB) == -1){
            secureFree(slavesPipes);
            perror("Creation pipe error");
        }

        pid_t pid;

        if ((pid = fork()) == 0) {

            close(channelA[1]); //cerrar la escritura porque solo leo
            close(channelB[0]); //cerrar la lectura porque solo escribo

            // En este for lo que hacemos es cerrar los fds creados por los demas slaves
            for (int j = 0; j < i; j++)
            {
                close(slavesPipes->readFD[j]);  
                close(slavesPipes->writeFD[j]);
            }
            // Guardo los pipes para que luego poder hacer el waite cuando terminen
            slavesPipes -> slavesIds[i] = pid;
            createChild("./esclavo.out", channelA[0], channelB[1]);

        } else {
            close(channelA[0]); //cerrar la lectura porque solo escribo channel A
            close(channelB[1]); //cerrar la escritura porque solo leo channel B

            slavesPipes->readFD[i] = channelB[0];
            slavesPipes->writeFD[i] = channelA[0];
        }

    }

    return slavesPipes;
}
void secureFreeSlave(slaves * freeElement){
    free(freeElement ->readFD);
    free(freeElement -> writeFD);
    free(freeElement->slavesIds);
    free(freeElement);
}
