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
    int slavesCount, filesNumber = argc - 1;
    Load * loads = getSlavesTasks(tasks, filesNumber, &slavesCount);
    
    /////////////////////////////////////////////////////////////
    /// Una vez que tengo las loads comienzo el slave manager y
    ///con eso empiezo a distribuir las tasks
    /////////////////////////////////////////////////////////////
    SlavesManager manager = createManager(slavesCount, argc-1);

    //inicialiso los iteradores de los files dentro de cada load
    initiAllIterators(loads, slavesCount); 

   //A todos los esclavos creados les doy de su carga inicial
   //el primer archivo
   for (int i = 0; i < manager->filesCount; i++) {

        //chequeo si tengo archivos, sino sigo
        if(!hasNextFileId(loads[i])) continue;

        //la load guarda no la string en si, sino un id
        //que viene a representar al indice en el arreglo argv
        int nextFileIdx = nextFileId(loads[i]);
        char * file = argv[nextFileIdx];    
        
        //por ultimo escribo al esclavo el archivo a traves
        //del slavesManager
        writeSlave(manager,file,i);
   }
   
    char message[MAXBUFFER]={0};
    while (getDoneFile(manager) < (argc-1)) {
  
        //Leo el output de un esclavo cualquiera a message
        readSlave(manager, message);

        //lo escribo al vista lo procesado por el esclavo
        write(STDOUT, message, strlen(message)+1);
        clearBuff(message);

        //idem del for loop anterior
        if(!hasNextFileId(loads[getLastView(manager)]))continue;
        int nextFileIdx = nextFileId(loads[getLastView(manager)]);
        char * file = argv[nextFileIdx];    

        writeSlave(manager,file,getLastView(manager));
        //? Falta cerrar los fd de los hijos y hacer los frees 
    }

    /////////////////////////////////////////////////////////////
    /// Por ultimo libero la memoria alocado dinamicamente y 
    /// espero a que terminen todos los esclavos que cree
    ///////////////////////////////////////////////////////////// 
    freeManager(manager);
    destroyAllLoads(loads, filesNumber);
    for(int status; wait(&status) != -1;)
  
 
    return 0;
}

