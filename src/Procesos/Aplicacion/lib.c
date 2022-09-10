// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../include/lib.h"

void * malloc_c(size_t size) {
    void * addr = malloc(size);
    if(addr == NULL) perror("malloc");
    return addr;
}
void clearBuff(char * toClear){
    for (int i = 0; toClear[i] != 0; i++)
    {
        toClear[i]=0;
    }
    
}
