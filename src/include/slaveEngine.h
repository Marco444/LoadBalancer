
#ifndef slave_Engine_H
#define slave_Engine_H
#include "loadDispatcher.h"
 /* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
/*
    Estructura la cual va a manejar la comunicacion con cada uno de los procesos esclavos
*/
#define MAXBUFFER 250

typedef struct {
    slaves * pipes;
    int filesCount;
    int filesDone;
    int slaveCount;
    int lastView;
    fd_set fdset;
    int inSet; 
}slavesManager;



#endif
