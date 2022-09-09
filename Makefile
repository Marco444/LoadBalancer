
FLAGS := -g -Wall -lpthread -lrt -std=c99 -D_XOPEN_SOURCE=500 -pedantic
LIB_PATH := ./src/Procesos/Aplicacion/lib.c
APLICACION_FILES := $(wildcard ./src/Procesos/Aplicacion/*.c)
ESCLAVO_FILES := ./src/Procesos/Esclavo/esclavo.c $(LIB_PATH)
VISTA_FILES:= ./src/Procesos/Vista/vista.c $(LIB_PATH) ./src/Procesos/Aplicacion/shmADT.c

all: APLICACION ESCLAVO VISTA
APLICACION:
	gcc $(FLAGS) -o "APLICACION"   $(APLICACION_FILES)
ESCLAVO: APLICACION
	gcc $(FLAGS) -o "ESCLAVO" $(ESCLAVO_FILES)
VISTA:ESCLAVO
	gcc $(FLAGS) -o "VISTA" $(VISTA_FILES)
clean:
	rm -f APLICACION ESCLAVO VISTA
			