// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/shmADT.h"
#include <stdio.h>

int main(){
    sleep(2);
    shmADT shm = openSHM("SHM_NAME", "SEM_NAME", O_RDWR, 0, 200, PROT_READ); 
    char buffer[200];
    readSHM(shm, buffer);
    printf(buffer);
    closeSHM(shm);
}