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

void reBalanceLoads(Load * loads, int loadNumber) {

    //ordeno los elementos por orden ASCENDENTE en el numero de
    //archivos, es decir mantengo 
    //qsort(loads, loadNumber, sizeof(struct task *), cmpLoad);



    return;
}

void readFilesInto(Task * tasks, char * argv[], int argc) {

    struct stat fileStats;    

    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &fileStats) != 0) {
            printf("Error! : cannot access %s file\n", argv[i]);
            exit(1);
        }

        if (S_ISDIR(fileStats.st_mode)) {
            printf("Error! : %s is a directory\n", argv[i]);
            exit(1);
        }

        
        tasks[i - 1] = malloc_c(sizeof(struct task));
        tasks[i - 1]->fileSize = fileStats.st_size;
        tasks[i - 1]->fileId = i; 
    }
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
    if(loadNumber > MAX_SLAVES) 
        reBalanceLoads(loads, loadNumber);

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
