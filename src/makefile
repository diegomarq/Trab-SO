all: processo_usr gerenciador substituidor shutdown clean

processo_usr: 
	gcc processo_usr.c -Wall -o processo_usr.out

gerenciador:
	gcc gerenciador.c -Wall -o gerenciador.out

substituidor:
	gcc substituidor.c -Wall -o substituidor.out

clean:
	rm -rf *.o	

fclean:
	rm -f *.out