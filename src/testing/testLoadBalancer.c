// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by Marco Scilipoti on 04/09/2022.
//



#include <printf.h>
#include "../TP1SO/Aplicacion/loadBalancer.h"


int main() {


    //Should divide it into 4 loads
    struct task task1 = { .fileSize = 2, .fileId = 1};
    struct task task2 = { .fileSize = 5, .fileId = 2};
    struct task task3 = { .fileSize = 4, .fileId = 3};
    struct task task4 = { .fileSize = 7, .fileId = 4};
    struct task task5 = { .fileSize = 1, .fileId = 5};
    struct task task6 = { .fileSize = 3, .fileId = 6};
    struct task task7 = { .fileSize = 8, .fileId = 7};

    Task tasks1[] = {&task1, &task2, &task3, &task4, &task5, &task6, &task7};

    int loadsCount = 0;
    Load * loads1 = slavesTasks(tasks1, 7, &loadsCount);

    for (int load = 0; load < loadsCount; ++load) {

        //empezamos el iterador para los archivos de la load actual
        initIterator(loads1[load]);

        //imprimo todos los filesIds
        printf("Los files de la load %d son: ", load);
        while(hasNextFileId(loads1[load])) {
            printf("%d", nextFileId(loads1[load]));
        }

        puts("\n");

        //libero la memoria utilizada por la load
        //destroyLoad(loads1[load]);
    }

    return 0;
}