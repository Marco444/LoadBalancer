
#include "../../include/esclavo.h"
int main(int argC,char * argV[]){

    char* ptr = (char*)malloc(MAXBUFFER);
    md5Calculate(ptr);
   // printf(ptr);
    free(ptr);
    int aux;
    return 0;
}
void md5Calculate(char * buffer){
    int fd[2];
    int returnV;
    if(pipe(fd) < 0)
        perror("Error to calculate md5");

    if ((returnV = fork()) == 0)
    {
        close(fd[0]);
        close(1);
        dup2(fd[1],1);
        close(fd[1]);
        execl("/bin/sh", "sh", "-c", "md5sum  aplicacion.c", (char *) 0);

    }else{
        if(returnV == -1)
            perror("Fork error in md5");
        else{
            int status;
            waitpid(returnV,&status,WUNTRACED | WCONTINUED);
            }
        sprintf(buffer,"%ld ",(long)getpid());
        read(fd[0],buffer + strlen(buffer),MAXBUFFER);
        close(fd[0]);
    }
}