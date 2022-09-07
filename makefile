
FLAGS = -Wall -fsanitize=address

loadBalancer:
			  	gcc $(FLAGS) main.c loadBalancer.c lib.c

loadDispatcher: slave.c, 
			   	gcc $(FLAGS) slave.c -o EXEC
			  	gcc $(FLAGS) testLoadDispatcher.c -o main
			  	./main

all:	Procesos/Aplicacion/aplicacion.c
				gcc $(FLAGS) esclavo.c -o ESCLAVO
