//
// Created by Marco Scilipoti on 04/09/2022.
//

#ifndef TP1SO_QUEUE_H
#define TP1SO_QUEUE_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

//La idea de esto va a ser una ADT para mantener una queue que va a ser necesaria par ir
//almacenando los archivos procesados por los esclavos. Como es necesario mantenerlos ordenados
//por orden de llegada utilisaremos una queue, que internamente es una linked list

struct Node{
    char * str;
    struct Node * next;
};

typedef struct Node  Node;

typedef struct {
    Node * first, * last;
    int length;
} Queue;

/* Recibe una queue, quita el primer elemento y lo devuelve.
 * Si no hay elementos hace un exit*/
Node * pop(Queue * queue);

/* Recibe una queue, agrega un elemento al final*/
void append(Queue * queue, char * elem);

/* Devuelve la longitud de la queue*/
int len(Queue * queue);

/* Inicializa una queue*/
Queue * initQueue();

/* Hace un dump de los elementos de la queue al fd recibido por argumento
 * de tal manera que libera la memoria dinamica*/
void dump(Queue * queue, FILE * fd);

int isEmpty(Queue * queue);

void destroyNode(Node * elem);

void destroyQueue(Queue * queue);

#endif //TP1SO_QUEUE_H
