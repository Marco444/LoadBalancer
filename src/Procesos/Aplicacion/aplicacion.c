#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <loadDispatcher.h>
#include <slaveEngine.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{

    if (argc < 2) printf("Usage: ./md5 <file1> <file2> ... <fileN>\n");

    /////////////////////////////////////////////////////////////
    /// Manejo la lectura de los archivos a un arreglo de Tasks, 
    /////////////////////////////////////////////////////////////
    struct stat fileStats;    
    Task tasks[argc];

    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &fileStats) != 0)
            printf("Error! : cannot access %s file\n", argv[i]);

        if (S_ISDIR(fileStats.st_mode))
            printf("Error! : %s is a directory\n", argv[i]);

        tasks[i] = malloc_c(sizeof(struct task));
        tasks[i]->fileSize = fileStats.st_size;
        tasks[i]->fileId = i;
    }

    /////////////////////////////////////////////////////////////
    /// Paso de las tasks a las loads para despues dispachear. 
    /////////////////////////////////////////////////////////////
    int slavesCount;
    Load * loads = getSlavesTasks(tasks, argc, &slavesCount);

    initiAllIterators(loads);
   
    /////////////////////////////////////////////////////////////
    /// Paso de las tasks a las loads para despues dispachear. 
    /////////////////////////////////////////////////////////////
    

    fd_set fdSet; // Preguntar si esto es necesario
    struct SlaveManager manager = {.slaveCount = slavesCount, .pipes = createSlaves(slavesCount), .fdset = &fdSet,.filesCount =  argc - 1, .filesDone = 0, .inSet = 0};
    
    char message[MAXBUFFER];

    while (manager.filesDone < manager.filesCount) {

        //Por cada elemeto que lee le envia uno a consiguiente
        readSlave(&manager, message);      


        if(!hasNextFileId(loads[manager.lastView])) continue;
        int nextFileIdx = nextFileId(loads[manager.lastView]);
        char * file = argv[nextFileIdx];    

        writeSlave(&manager,file,manager.lastView);
    }

    destroyAllLoads(loads);

    int status;
    for (int i = 0; i < manager.slaveCount; i++)
    {
        wait(&status);
    }
    
}
