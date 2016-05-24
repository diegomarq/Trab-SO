H = ./header/

all: processo_usr gerenciador substituidor shutdown clean

processo_usr: 
	gcc -Wall processo_usr.c $(H)src.h -o processo_usr.out

gerenciador:
	gcc -Wall gerenciador.c $(H)src.h -o gerenciador.out

substituidor:
	gcc -Wall substituidor.c $(H)src.h -o substituidor.out

clean:
	rm -rf *.o	

fclean:
	rm -f *.out