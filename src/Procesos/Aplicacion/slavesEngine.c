//
// Created by Marco Scilipoti on 05/09/2022.
//

#include "../../include/loadDispatcher.h"
#include "../../include/lib.h"
#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void createChild(char * file, int read_addr, int write_addr) {

    // seteo de la entrada y salida entrada del proyecto esclavo
    dup2(read_addr,0);
    dup2(write_addr,1);
    close(read_addr);
    close(write_addr);
    //define string to hold both fds -> Aca le debemos mandar los primeros archivos que debe desarrollar el slave
    char * fds[] = {read, write};
    char * childArgs[]={file, (char *) fds, NULL};

    execv(file, childArgs);
}

slaves * createSlaves(int slaveCount)
{
    //TODO habria que ver si se podria cambiar de lugar esto asi no creo inecesariamente con malloc
    slaves * slavesPipes = malloc_c(sizeof(slaves));              
    slavesPipes -> readFD = malloc_c(sizeof(int) * slaveCount);
    slavesPipes -> writeFD = malloc_c(sizeof(int) * slaveCount);
    for (int i = 0; i < slaveCount; ++i) {

        //definimos el channelA para que el padre escriba y el hijo lea
        int channelA[2];
        //definimos le channelB para que el padre lea y el hijo escriba
        int channelB[2];

        if(pipe(channelA) == -1 ||  pipe(channelB) == -1){
            secureFreeSlave(slavesPipes);
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
            createChild("../Esclavo/ESCLAVO", channelA[0], channelB[1]);

        } else {
            close(channelA[0]); //cerrar la lectura porque solo escribo channel A
            close(channelB[1]); //cerrar la escritura porque solo leo channel B

            slavesPipes->readFD[i] = channelB[0];
            slavesPipes->writeFD[i] = channelA[1];
        }

    }

    return slavesPipes;
}
void secureFreeSlave(slaves * freeElement){
    free(freeElement ->readFD);
    free(freeElement -> writeFD);
    free(freeElement);
}
