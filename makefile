all: processo_usr alocador substituidor shutdown clean

processo_usr:
	gcc -Wall processo_usr.c utilidades.h -o processo_usr.out

alocador: semaforo.o
	gcc -Wall alocador.c utilidades.h semaforo.o -o alocador.out

substituidor: semaforo.o
	gcc -Wall substituidor.c utilidades.h semaforo.o -o substituidor.out

shutdown: semaforo.o
	gcc -Wall shutdown.c utilidades.h semaforo.o -o shutdown.out

semaforo.o:
	gcc -Wall semaforo.c -c

clean:
	rm -rf *.o	

fclean:
	rm -f *.out