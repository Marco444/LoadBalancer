

/*

    Es aca en donde voy  tener que chequear si algun hijo me escribio y cuando escribirle y esas cosas

*/

#include <loadDispatcher.h>
int next(fd_set *fdset, int lastView, int *readFd, int count);
void setFDS(int *fds, fd_set *fdset, int count);

void getDone(slavesManager * manager, char *buffer)
{

    if (manager->inSet == 0)
    {
        setFDS(manager->pipes->readFD, manager->fdset, manager->slaveCount);
        if ((manager->inSet = select(manager->slaveCount,manager->fdset, NULL, NULL, NULL)) == -1){
            perror("Error in select");
            exit(1);
            }
        else
            manager->lastView = 0;
    }

    int index = next(manager->fdset, manager->lastView, manager->pipes->readFD, manager->slaveCount);
    manager->inSet -= 1;
    manager->lastView = index;
    FD_CLR(manager->pipes->readFD[manager->lastView],manager->fdset);
    read(manager->pipes->readFD[index], buffer, MAXBUFFER);
}

void writeSlave(slavesManager *manager, char *file, int slaveNum)
{
    if (write(manager->pipes->writeFD[slaveNum], file, MAXBUFFER) == -1)
        perror("Writting error");
}

int next(fd_set *fdset, int lastView, int *readFd, int count)
{
    for (int i = lastView; i < count; i++)
    {
        if (FD_ISSET(readFd[i], fdset))
            return i;
    }
    return -1;
}
void setFDS(int *fds, fd_set *fdset, int count)
{
    FD_ZERO(fdset);
    for (int i = 0; i < count; i++)
    {
        FD_SET(fds[i], fdset);
    }
}