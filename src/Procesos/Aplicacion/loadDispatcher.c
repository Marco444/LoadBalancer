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
    sprintf(write, "%d", write_addr);

    //define string to hold both fds
    char * fds[] = {read, write};
    char * childArgs[]={"./EXEC", (char *) fds, NULL};

    execv(file, childArgs);
}

Slave * createSlaves(int slaveCount)
{
    Slave * slaves = malloc(slaveCount * sizeof(Slave));

    for (int i = 0; i < slaveCount; ++i) {

        //definimos el channelA para que el padre escriba y el hijo lea
        int channelA[2];
        pipe(channelA);

        //definimos le channelB para que el padre lea y el hijo escriba
        int channelB[2];
        pipe(channelB);

        pid_t pid;

        if ((pid = fork()) == 0) {

            close(channelA[1]); //cerrar la escritura porque solo leo
            close(channelB[0]); //cerrar la lectura porque solo escribo

            createChild("./EXEC", channelA[0], channelB[1]);

        } else {
            wait(&pid);

            close(channelA[0]); //cerrar la lectura porque solo escribo channel A
            close(channelB[1]); //cerrar la escritura porque solo leo channel B

            slaves[i]->read_address = channelB[0];
            slaves[i]->write_address = channelA[0];
        }

    }

    return slaves;
}
