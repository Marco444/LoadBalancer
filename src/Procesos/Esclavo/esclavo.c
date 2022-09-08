
#include "../../include/esclavo.h"
#include "../../include/lib.h"
void clearBuff(char * toClear);
int main(int argC, char *argV[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    char ptr[MAXBUFFER]={0};
    // Le voy calculando todos los md5 a los archivos iniciales
    // cuando termino con ellos pido mas
    for (int i = 0; i < (argC - 1); i++)
    {
        md5Calculate(ptr,argV[i+1]);
        printf(ptr);
        
    }
    char * actFile = NULL;
    size_t size = 0;
    // Si es null quiere decir que se cerro el pipe
    while (getline(&actFile,&size,stdin) != -1)
    {     
        clearBuff(ptr);
        md5Calculate(ptr,actFile);
        clearBuff(actFile);
        printf("%s",ptr);
        
    }
    return 0;
}

void md5Calculate(char *buffer,char * file)
{
    int fd[2];
    int childPid;
    int status;
    char command[MAXBUFFER];
    if (pipe(fd) < 0)
        perror("Error to calculate md5");

    // Esto se puede cambiar por un popen 
    if ((childPid = fork()) == 0)
    {
        close(fd[0]);
        close(1);
        dup2(fd[1], 1);
        close(fd[1]);
        sprintf(command,"%s %s","md5sum",file);
        execl("/bin/sh", "sh", "-c", command , (char *)0);
    }
    else
    {
        if (childPid == -1)
            perror("Fork error in md5");
        close(fd[1]);   // Padre solo lee
        waitpid(childPid, &status, WUNTRACED | WCONTINUED); // Espero a la respuesta del hijo 
        sprintf(buffer, "%ld ", (long)getpid());            // Concateno
        read(fd[0], buffer + strlen(buffer), MAXBUFFER);    // leo lo que me deja el hijo
        close(fd[0]);
    }
}

