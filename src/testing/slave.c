// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by Marco Scilipoti on 05/09/2022.
//


#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    //defino las dos direcciones donde voy a leer y donde voy a escribir como esclavo.
    int read_address = atoi(argv[0]), write_address = atoi(argv[1]);


    char batch[10];
    read(read_address, batch, 10);

    printf("\n");

    return 0;
}