#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x6120

if ((idsem = semget(0x6120, 1, IPC_CREAT|0x1ff)) < 0) {
	printf("Erro na criacao do semaforo\n");
	exit(1);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define NUMERO_FRAMES 10
#define MAX_OCUPACAO 9
#define	OCUPACAO_OK 8
#define KEY_MSG 0x6121

typedef struct frame{
	int processOwner; 	//The process owner of the frames
	int pageNumber; 	//Number of the process page
	int frameUsed; 		//1 for used frames, 0 for empty frames
	int LRU; 			//Integer that represents the last time that the page was referenced
}Frame;

typedef struct requisicao{
	long pid;
	int page;
}Requisicao;

Frame TB_pages[NUMERO_FRAMES];
Requisicao teste;
/*
void resetFrameUsed() {
	for (int i = 0; i < NUMERO_FRAMES; ++i){
		TB_pages[i].frameUsed = 0;
	}for
}

void copyFrame(int line, Frame data){
	TB_pages[i].process = data.process;
	TB_pages[i].page = data.page;
	TB_pages[i].used = 1;
}

int referencia_pagina() {
	if(pages < MAX_OCUPACAO) {
		//percorre a tabela de paginas
		//encontra uma vazia
		//mapeia a page
		for (i = 0; i < NUMERO_FRAMES; i++){
			if(TB_pages[i].used == 0) {
				copyFrame();
				break;
			}
		}
	} else {
		libera_pagina();
	}

}*/
int main () {
	//Criar fila de msg;

	int idfila;

	if (idfila = msgget(KEY_MSG, IPC_CREAT | 0x1FF) < 0) {
		printf("erro na criacao da fila\n");
		exit(1);
	}
	msgrcv(idfila, &teste, sizeof(Requisicao), 0, 0);
	printf("processo: %ld\t pagina: %d\n", teste.pid, teste.page);
	printf("%d ger\n", idfila);
	//msgctl(idfila, IPC_RMID, 0);
	// while(1){

	// }
	//Loop infinito esperando requisicao;
	//Encerra programa;
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_MSG 0x6121

typedef struct requisicao{
	long pid;
	int page;
}Requisicao;

	Requisicao req_msg;

int main () {

	int idfila;
	req_msg.pid = 123;
	req_msg.page = 10;

	printf("%ld\t%d\n", req_msg.pid, req_msg.page);

	if (idfila = msgget(KEY_MSG, 0x124) < 0) {
		printf("erro na obtencao da fila\n");
		exit(1);
	}
	printf("%d usr\n", idfila);
	msgsnd(idfila, &req_msg, sizeof(Requisicao), 0);
	return 0;
}