//
// Created by Marco Scilipoti on 04/09/2022.
//

#ifndef TEST_LOADBALANCER_H
#define TEST_LOADBALANCER_H


/*
 * El problema que busca resolver se base en, cuantos esclavos tengo que tener para procesar los archivos? La respuesta
 * a esta pregunta tiene muchas aristas pero la primera es que el tiempo de hashear md5 todos los archivos esta
 * limitado por el archivo de mayor tamnanio.
 *
 * (Sin importar la cantidad de esclavos, asumiendo md5 no distribuido, el procesamiento de todos los archivos no puede tomar menos tiempo
 * que el tiempo que toma el archivo mas grande. Al mismo tiempo, la complejidad temporal del md5 esta dada por la cantidad
 * de bits del archivo, es decir que el archivo que va a tardar mas va a ser el de mayor tamanio).
 *
 * Investigando un poco encontramos que era un problema conocido de EDA (firstFit) y asi implementamos esta
 * estimacion para el minimo numero de esclavos tal que cada esclavo tenga que ejecutar el archivo mas grande
 * o archivos mas chicos cuyo tamanio no supera el archivo mayor. De esta manera maximizar el valor que provee
 * la capacidad de paralelizacion del sistema.
 *
 * Luego procedimos a expandir sobre esto y no solamente definir el numero de esclavos que tiene que haber, pero
 * tambien los archivos que deberian procesar a medida que van terminando los batches que van haciendo.
 *
 * Por ultimo, implementamos un metodo 
 * */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SLAVES 50
//Buscamos el limite del proceso pero ulimit -u me da: unlimited resources
//por lo tanto seteamos a 50 como una buena aproximacion del maximo numero
//de procesos. (Luego de investigar un poco mas entendimos como en terminos
//generales la cantidad de forks esta limitada por la memoria del sistema,
//tomamos 50 como una aproximacion para la mayoria de maquinas modernas) 

#define OVERLOADING_FACTOR 3
//Este factor viene a definir cuanto veces mas puede pesar una Load (sin rebalancear)
//que el archivo mas pesado. Es una opcion que puede permitir optimizar el codigo, 
//a partir de los tamnios que recibe el programa y el tiempo que toma hacer un esclavo mas.

typedef int FileId;

///Un node viene a ser un elemento de la lista encadenada que mantenemos para los fileIds
///que componen a una load.
typedef struct nodeFileId {
    FileId val;
    struct nodeFileId * next;
} * NodeFileId;

///Una task viene a ser una tarea para nuestros esclavos, definida por el tamanio del archivo asi como
///el fileId del mismo para despues mapearlo al esclavo.
typedef struct task {
    size_t fileSize;
    size_t fileId;
} * Task;

///Una load viene a ser la carga de trabajos que idealmente seria asignada a un unico esclavo,
///compuesta del tamanio que tiene y la lista de filesIds que la componen.
typedef struct load {
    size_t size, fileCount;                   //mantiene el tamanio de la carga del esclavo de trabajo
    NodeFileId first, last, current;          //mantiene una lista con los ids de los files que procesaria el esclavo.
} * Load ;

/* recibe un arreglo con el tamanio de los archivo asi como el nombre del archivo en cuestion,
 * y el numero de archivos a procesar. Devuelve un arreglo donde cada indice corresponde a un
 * esclavo diferente y mantiene los fileIds que tienen que procesar el esclavo.
 * */
Load * getSlavesLoads(Task * tasks, int count, int * loadsCount);

void reBalanceLoads(Load * loads, int loadNumber);

/* recibe los argumentos con los que se llamo a main y con ellos completa las tasks que se
 tirenen que hacer */
void readFilesInto(Task * tasks, char ** files, char * argv[], int argc, int * filesCount);

/* adds the task in question to the load */
void appendTask(Load load, Task task);

/* devuelve si es valido el hecho de agregar la task a la load de un esclavo en particular. Aca viene la clave del
 * load balancer, esta es la funcion que determina si una load "balancea" al agregarle una task o no. */
int loadBalances(Load load, Task task, Task * tasks, int taskCount);

/* agrega el elemento en cuestion a la lista encadenada */
void addFileId(Load load, int val);

/* devuelve si tiene un file Id todavia por hacerse*/
int hasNextFileId(Load load);

/* devuelve el siguiente fileId almacenado en la lista*/
int nextFileId(Load load);

/* destruye toda la informacion atada a una load */
void destroyLoad(Load load);

/* destruye toda la informacion atada a cada task */
void destroyTasks(Task * tasks, int count);

/* comienza el iterador de las laods*/
void initIterator(Load load);

/* comparador para ordenar taks en orden DESCENDENTE */
int cmpTask (const void * a, const void * b);

/* comienza todos los iteradores de un grupo de loads */
void initiAllIterators(Load * loads, int count);

/* destruye toda informacion alocada dinamicamente a las Loads */
void destroyAllLoads(Load * loads, int count);

#endif //TEST_LOADBALANCER_H
