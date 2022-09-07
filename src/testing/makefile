
FLAGS = -Wall -fsanitize=address

loadBalancer:
			  	gcc $(FLAGS) main.c loadBalancer.c lib.c

loadDispatcher: slave.c, 
			   	gcc $(FLAGS) slave.c -o EXEC
			  	gcc $(FLAGS) testLoadDispatcher.c -o main
			  	./main

parser: ../Procesos/Aplicacion/aplicacion.c
				gcc $(FLAGS) ../Procesos/Aplicacion/aplicacion.c
							../Procesos/Aplicacion/loadBalancer.c
							../Procesos/Aplicacion/loadDispatcher.c
							../Procesos/Aplicacion/slavesEngine.c
