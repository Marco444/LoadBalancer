// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by Marco Scilipoti on 04/09/2022.
//

#include "../../include/loadBalancer.h"
#include "../../include/lib.h"

Load * createLoads(int loadsCount) {

    Load * loads = malloc_c(loadsCount * sizeof(struct load *));

    for (int i = 0; i < loadsCount; i++) {
        loads[i] = malloc_c(sizeof(struct load));
        loads[i]->fileCount = 0;
        loads[i]->size = 0;
    }

    return loads;
}

int cmpLoad(const void * a, const void * b) {
    return  (int) (((*(struct load * *) a))->fileCount) - (int) (((*(struct load * *) b))->fileCount);
}

void reBalanceLoads(Load * loads, int loadNumber) {

    //ordeno los elementos por orden ASCENDENTE en el numero de
    //archivos. (Para ver la justificacion completa ver el informe,
    //pero la idea es que los archivos que mas importa distribuir 
    //bien son los mas pesados, es decir las tasks que tienen 
    //menor numero de archivos no se deberian tocar)/
    qsort(loads, loadNumber, sizeof(struct task *), cmpLoad);

    //indice para mantener la load donde estoy cargando 
    int idx = 0;

    for (size_t i = MAX_SLAVES - 1; i < loadNumber; i++) {

        //empiezo a iterar por la load que hay que redistribuir
        initIterator(loads[i]);

        //copio todos los archivos de la load que tengo que redistribuir
        //a las loads que puedo dispatchear.
        while(hasNextFileId(loads[i])) {

            //paso la task de la load que no puedo generarle un esclavo a otra
            addFileId(loads[idx], nextFileId(loads[i])); 

            //me muevo a la siguiente load a donde puedo agregarle otra task.
            if(idx == MAX_SLAVES) idx = 0;
            else idx++;
        }
    }

}

void readFilesInto(Task * tasks, char ** files, char * argv[], int argc, int * fileCount) {

    struct stat fileStats;    

    int idx = 0;

    for (int i = 1; i < argc; i++) {
        tasks[i - 1] = malloc_c(sizeof(struct task));

        //si es directorio o no se puede abrir tomo que no tiene tamanio
        if((stat(argv[i], &fileStats) != 0 || S_ISDIR(fileStats.st_mode)))
            tasks[i - 1]->fileSize = 0;
        else {
            tasks[i - 1]->fileSize = fileStats.st_size;
           // files[idx++] = argv[i];
        }

        tasks[i - 1]->fileId = i; 
    }

    *fileCount = idx;
}


Load * getSlavesLoads(Task * tasks, int taskCount, int * loadsCount) {

    //ordeno los elementos por orden DESCENDENTE
    qsort(tasks, taskCount, sizeof(struct task *), cmpTask);

    //mantenemos un contador para la cantidad de loads que tuvimos
    //que crear (luego las mapeamos a los esclavos).
    int loadNumber = 0;

    //definimos la lista de loads, que como maximo va a ser el
    //numero de tasks que tenemos que manejar.
    Load * loads = createLoads(taskCount);

    //Vamos por cada task de nuestra lista
    for (int i = 0; i < taskCount; i++) {

        //luego vamos por cada una de las loads ya utilizadas y vemos si en la
        //load entra la task en cuestion.
        int load = 0;
        while(load < loadNumber && !loadBalances(loads[load], tasks[i], tasks, taskCount)) load++;

        //luego la agrego
        appendTask(loads[load], tasks[i]);

        //y verifico que si es igual al loadNumber que tengo actualmente
        //entonces no entro en ninguna load ya definida, por lo tanto tengo
        //que aumentar uno al número de loads.
        if(load == loadNumber) loadNumber++;
    }

    //updateamos la cantidad de loads que creamos
    *loadsCount = loadNumber;

    //destruimos la memoria alocada para tasks
    destroyTasks(tasks, taskCount);

    //en el caso que no tengo suficientes esclavos para hacer uno por
    //load, re-balanceo las loads entre estos.
    if(loadNumber > MAX_SLAVES) { 
        reBalanceLoads(loads, loadNumber);
        *loadsCount = MAX_SLAVES;
    }

    return loads;
}

void appendTask(Load load, Task task) {

    //actualizamos el tamanio de nuestra load al agregarle una task
    load->size += task->fileSize;

    //agregamos el fileId de la task a la load
    addFileId(load, (int) task->fileId);
}

int loadBalances(Load load, Task task, Task * tasks, int taskCount) {

    //como ya definimos el tamanio maximo de una load no deberia ser mayor al
    //tamanio de la task mas grande, tambien considerando un factor de overloading
    return tasks[taskCount - 1]->fileSize >  task->fileSize +  load->size;
}

void destroyTasks(Task * tasks, int count) {
    for (size_t i = 0; i < count; i++)
        free(tasks[i]);
}

void destroyLoad(Load load) {

    while(load->first != NULL) {
        NodeFileId toFree = load->first;
        load->first = load->first->next;
        free(toFree);
    }

    free(load);
}

void addFileId(Load load, FileId fileId) {

    //creamos el nuevo elemento de la lista
    NodeFileId newGuy = malloc_c(sizeof(struct nodeFileId));
    newGuy->val = fileId;
    newGuy->next = NULL;

    //chequeamos si es el primer elemento
    if(load->fileCount == 0) {
        load->first = newGuy;
        load->last = newGuy;

    //sino lo es, simplemente lo agregamos al final
    } else {
        load->last->next = newGuy;
        load->last = load->last->next;
    }

    load->fileCount++;
}

void initIterator(Load load) {
    load->current = load->first;
}

int hasNextFileId(Load load) {
    return load->current != NULL;
}

int nextFileId(Load load) {
    if(!hasNextFileId(load)) perror("no more file ids to iterate");

    int fileId = load->current->val;
    load->current = load->current->next;
    return fileId;
}

int cmpTask (const void * a, const void * b) {
    return  (int) (((*(struct task * *) a))->fileSize) - (int) (((*(struct task * *) b))->fileSize);
}

void initiAllIterators(Load * loads, int count) {
    for (size_t i = 0; i < count; i++)
        initIterator(loads[i]);
}

void destroyAllLoads(Load * loads, int count) {
    for (size_t i = 0; i < count; i++)
        destroyLoad(loads[i]);
    free(loads);
}
