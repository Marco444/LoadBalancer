
FLAGS := -Wall -fsanitize=address -std=c99 -lrt -D_XOPEN_SOURCE=500 -pedantic -g -lm 
APLICACION_FILES := $(wildcard ./src/Procesos/Aplicacion/*.c)
ESCLAVO_FILES := ./src/Procesos/Esclavo/esclavo.c ./src/Procesos/Aplicacion/lib.c
VISTA_FILES:= ./src/Procesos/Vista/vista.c ./src/Procesos/Aplicacion/lib.c ./src/Procesos/Aplicacion/shmADT.c
APLICACION:
	gcc $(FLAGS) -o "APLICACION"   $(APLICACION_FILES)

ESCLAVO: esclavo.c 
	gcc $(FLAGS) -o "ESCLAVO" $(ESCLAVO_FILES)

VISTA: vista.c
	gcc $(FLAGS) -o "VISTA" $(VISTA_FILES)


			