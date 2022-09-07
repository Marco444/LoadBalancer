
FLAGS = -Wall -fsanitize=address

loadBalancer:
			  	gcc $(FLAGS) main.c loadBalancer.c lib.c

loadDispatcher: slave.c, 
			   	gcc $(FLAGS) slave.c -o EXEC
			  	gcc $(FLAGS) testLoadDispatcher.c -o main
			  	./main

all:
				gcc -lpthread -std=c99 -o "APLICACION" aplicacion.c loadBalancer.c loadDispatcher.c slavesEngine.c lib.c
