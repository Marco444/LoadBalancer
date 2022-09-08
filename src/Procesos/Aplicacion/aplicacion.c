#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "../../include/slavesEngine.h"
#include "../../include/loadDispatcher.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "../../include/loadBalancer.h"
#include "../../include/lib.h"
#include <sys/select.h>
void clearBuff(char * toClear);
int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    if (argc < 2) {
        printf("Usage: ./md5 <file1> <file2> ... <fileN>\n");
        exit(1);
    }

    /////////////////////////////////////////////////////////////
    /// Manejo la lectura de los archivos a un arreglo de Tasks, 
    /////////////////////////////////////////////////////////////
    Task tasks[argc - 1];
    readFilesInto(tasks, argv, argc);

    /////////////////////////////////////////////////////////////
    /// Paso de las Tasks a las Loads para despues dispachear. 
    /////////////////////////////////////////////////////////////
    int slavesCount;
    Load * loads = getSlavesTasks(tasks, argc - 1, &slavesCount);

    initiAllIterators(loads, slavesCount); 
    
    /////////////////////////////////////////////////////////////
    /// Una vez que tengo las loads comienzo el slave manager y
    ///con eso empiezo a distribuir las tasks
    /////////////////////////////////////////////////////////////
   SlavesManager manager = createManager(slavesCount,argc-1);
   
   //Give initial tasks
   for (int i = 0; i < manager->filesCount; i++)
   {
        if(!hasNextFileId(loads[i])) continue;
        int nextFileIdx = nextFileId(loads[i]);
        char * file = argv[nextFileIdx];    
        writeSlave(manager,file,i);
   }
   
    char message[MAXBUFFER]={0};
    while (getDoneFiles(manager) < (argc-1)) {
  
        //Por cada elemeto que lee le envia uno a consiguiente
        readSlave(manager, message);
        write(1,message,strlen(message)+1);
        clearBuff(message);
        if(!hasNextFileId(loads[getLastView(manager)]))continue;
         
        int nextFileIdx = nextFileId(loads[getLastView(manager)]);
        char * file = argv[nextFileIdx];    
        writeSlave(manager,file,getLastView(manager));
        //Falta cerrar los fd de los hijos y hacer los frees
    }

    freeManager(manager);
    destroyAllLoads(loads,slavesCount);

    int status;
    for (int i = 0; i < manager->slaveCount; i++)
    {
        wait(&status);
    }
 
    return 0;
}

