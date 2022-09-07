#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <loadDispatcher.h>
#include <slaveEngine.h>


int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: ./md5 <file1> <file2> ... <fileN>\n");
    }

    struct stat stats;
    for (int i = 1; i < argc; i++)
    {
        if (stat(argv[i], &stats) != 0)
            printf("Error! : cannot access %s file\n", argv[i]);

        if (S_ISDIR(stats.st_mode))
            printf("Error! : %s is a directory\n", argv[i]);
    }


    fd_set fdSet; // Preguntar si esto es necesario
    struct SlaveManager manager = {.slaveCount = 15, .pipes = createSlaves(3), .fdset = fdSet,.filesCount =  argc - 1, .filesDone = 0, .inSet = 0};
    char message[MAXBUFFER];
    char file[MAXBUFFER]; // esto es representativo
    while (manager.filesDone < manager.filesCount)
    {
        //Por cada elemeto que lee le envia uno a consiguiente
        getDone(&manager, message);             
        writeSlave(&manager,file,manager.lastView);
    }
}
