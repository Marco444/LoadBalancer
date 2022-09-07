//
// Created by Marco Scilipoti on 04/09/2022.
//

#ifndef TEST_LIB_H
#define TEST_LIB_H

#include <stdlib.h>
#include <stdio.h>

#define MAXBUFFER 255

//____SHM NAMES____
#define SHM_NAME "myShm"
#define SEM_NAME "mySem"

/* un wrapper del malloc que chequea si fallo o no */
void * malloc_c(size_t size);




#endif //TEST_LIB_H
