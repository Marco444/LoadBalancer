#include "../include/shmADT.h"

int main(){

    shmADT shm = createSHM("SHM_NAME", "SEM_NAME", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 200, PROT_WRITE);
    writeSHM(shm, "Hola! Esto esta en shm\n");   

    sleep(10);
    closeSHM(shm);
}