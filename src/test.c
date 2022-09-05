#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
int main(int argc, char * argv[]){

    sem_t *semaphore = sem_open("Example", O_CREAT, 0644, 0);
     if(semaphore == (void*) -1){
         printf("sem_open failure");
     }
    sem_wait(semaphore);

    printf("Its in the child \n");

    sem_post(semaphore);
    return 0;
}