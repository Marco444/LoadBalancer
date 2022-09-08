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
#include "../../include/shmADT.h"
#include <sys/mman.h>
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
    //  Abrimos un espacio de memoria compartida (SHM).
    /////////////////////////////////////////////////////////////
    shmADT shm = createSHM(SHM_NAME, SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, MAXBUFFER * (argc - 1), PROT_WRITE);
    printf("%d", argc - 1);
    sleep(10);
    /////////////////////////////////////////////////////////////
    /// Una vez que tengo las loads comienzo el slave manager y
    ///con eso empiezo a distribuir las tasks
    /////////////////////////////////////////////////////////////
   SlavesManager manager = createManager(slavesCount,argc-1);
   for (int i = 0; i < manager->filesCount; i++)
   {
        if(!hasNextFileId(loads[i])) continue;
        int nextFileIdx = nextFileId(loads[i]);
        char * file = argv[nextFileIdx];    
        writeSlave(manager,file,i);
   }
   
    char message[MAXBUFFER]={0};
    while (manager ->filesDone < manager->filesCount) {
        clearBuff(message);
        //Por cada elemeto que lee le envia uno a consiguiente
        readSlave(manager, message);
        //write(1,message,strlen(message)+1);
        writeSHM(shm, message);
        clearBuff(message);
        if(!hasNextFileId(loads[manager->lastView]))continue;
         
        int nextFileIdx = nextFileId(loads[manager->lastView]);
        char * file = argv[nextFileIdx];    
        writeSlave(manager,file,manager->lastView);
        //Falta cerrar los fd de los hijos y hacer los frees
    }
    secureFreeSlave(manager->pipes,manager->slaveCount);
    destroyAllLoads(loads,slavesCount);

    int status;
    for (int i = 0; i < manager->slaveCount; i++)
    {
        wait(&status);
    }
    
    closeSHM(shm);
    
    return 0;
}
void clearBuff(char * toClear){
    for (int i = 0; toClear[i] != 0; i++)
    {
        toClear[i]=0;
    }
    
}
