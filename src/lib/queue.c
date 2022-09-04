//
// Created by Marco Scilipoti on 04/09/2022.
//

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

Queue * initQueue() {
    Queue * newGuy = malloc(sizeof(Queue));
    newGuy->last = NULL;
    newGuy->first = NULL;
    newGuy->length = 0;
    return newGuy;
}

/* Recibe una queue, quita el primer elemento y lo devuelve.
 * Devuelve -1 si esta vacia*/
char * pop(Queue queue) {

    if(queue.length <+ 0){
        perror("No hay elementos a popear");
        exit(1);
    }

    node * elem = queue.first;
    queue.first = queue.first->next;
    queue.length--;

    return elem->str;
}

/* Recibe una queue, agrega un elemento al final*/
void append(Queue queue, char * str) {

    //creamos el nuevo elemento de la lista
    node * newGuy = malloc(sizeof(node));
    newGuy->str = str;
    newGuy->next = NULL;

    //chequeamos si es el primer elemento
    if(queue.length == 0) {
        queue.first = newGuy;
        queue.last = newGuy;
    //sino lo es, simplemente lo agregamos al final
    } else {
        queue.last->next = newGuy;
        queue.last = queue.last->next;
    }

    queue.length++;
}

/* Devuelve la longitud de la queue*/
int len(Queue queue) {
    return queue.length;
}