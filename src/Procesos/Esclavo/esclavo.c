// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../../include/esclavo.h"
#include "../../include/lib.h"
void clearBuff(char * toClear);
int main(int argC, char *argV[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    char ptr[MAXBUFFER]={0};
    char * actFile = NULL;
    size_t size = 0;
    // Si es null quiere decir que se cerro el pipe
    while (getline(&actFile,&size,stdin) != -1)
    {     
        clearBuff(ptr);
        md5Calculate(ptr,actFile);
        clearBuff(actFile);
        printf(ptr);
        
    }
    return 0;
}

void md5Calculate(char *buffer,char * file)
{       struct stat fileStats;    

       if (stat(file, &fileStats) != 0) {
            sprintf(buffer,"Error! : cannot access %s file\n", file);
            return;
        }else if(S_ISDIR(fileStats.st_mode)){
             sprintf(buffer,"Error! : Is a directory %s\n", file);
             return;
        }

    char command[MAXBUFFER];
    sprintf(command,"%s %s","md5sum",file);
    FILE * outPut = popen(command,"r");
    sprintf(buffer, "%ld ", (long)getpid());            // Concateno
    fgets( (buffer + strlen(buffer)),MAXBUFFER,outPut);    // leo lo que me deja el hijo
    pclose(outPut);
}

