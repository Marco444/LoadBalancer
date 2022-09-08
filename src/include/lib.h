//
// Created by Marco Scilipoti on 04/09/2022.
//

#ifndef LIB_H
#define LIB_H

#include <stdlib.h>
#include <stdio.h>

#define MAXBUFFER 255

//____SHM NAMES____
#define SHM_NAME "myShm"
#define SEM_NAME "mySem"

/* un wrapper del malloc que chequea si fallo o no */
void * malloc_c(size_t size);
/* function for clear a char * */
void clearBuff(char * toClear);


#endif //TEST_LIB_H
