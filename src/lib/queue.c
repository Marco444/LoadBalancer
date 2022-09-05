//
// Created by Marco Scilipoti on 04/09/2022.
//

#include "../include/queue.h"
#include "../../../test/lib.h"


Queue * initQueue() {
    Queue * newGuy = malloc_c(sizeof(Queue));
    newGuy->last = NULL;
    newGuy->first = NULL;
    newGuy->length = 0;
    return newGuy;
}


Node * pop(Queue * queue) {

    if(queue->length <+ 0){
        perror("No hay elementos a popear");
        exit(1);
    }

    Node * elem = queue->first;
    queue->first = queue->first->next;
    queue->length--;

    return elem;
}

void append(Queue * queue, char * str) {

    //creamos el nuevo elemento de la lista
    Node * newGuy = malloc_c(sizeof(Node));
    newGuy->str = str;
    newGuy->next = NULL;

    //chequeamos si es el primer elemento
    if(queue->length == 0) {
        queue->first = newGuy;
        queue->last = newGuy;
    //sino lo es, simplemente lo agregamos al final
    } else {
        queue->last->next = newGuy;
        queue->last = queue->last->next;
    }

    queue->length++;
}

void destroyNode(Node * elem) {
    free(elem->str);
    free(elem);
}

void destroyQueue(Queue * queue) {
    while(!isEmpty(queue))
        destroyNode(pop(queue));
    free(queue);
}

void dump(Queue * queue, FILE * fd) {

    while(!isEmpty(queue)) {
        Node * elem = pop(queue);
        fprintf(fd, "%s, ", elem->str);
        destroyNode(elem);
    }
}


int len(Queue * queue) {
    return queue->length;
}

int isEmpty(Queue * queue) {
    return queue->length <= 0;
}