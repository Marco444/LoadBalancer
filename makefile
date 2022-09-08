
FLAGS = -Wall -fsanitize=address

loadBalancer:
			  	gcc $(FLAGS) main.c loadBalancer.c lib.c

loadDispatcher: slave.c, 
			   	gcc $(FLAGS) slave.c -o EXEC
			  	gcc $(FLAGS) testLoadDispatcher.c -o main
			  	./main

all:
				gcc -g -lpthread -lrt -std=c99 -D_XOPEN_SOURCE=500 -pedantic -o "APLICACION" aplicacion.c loadBalancer.c loadDispatcher.c slavesEngine.c lib.c shmADT.c
				gcc -g -Wall -lm -lrt -lpthread -std=c99 -D_XOPEN_SOURCE=500 -pedantic -o "VISTA" vista.c lib.c shmADT.c
				gcc -g -lpthread -std=c99 -o ESCLAVO ../Esclavo/esclavo.c lib.c

			