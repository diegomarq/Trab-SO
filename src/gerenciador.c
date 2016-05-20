#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>

#define NUMERO_FRAMES 30
#define MAX_OCUPACAO 9
#define	OCUPACAO_OK 8
#define KEY_MSG 0x1234
#define KEY_MEM_TB 0x4321
#define KEY_MEM_NP 0x1100

typedef struct frame{
	int processOwner; 	//The process owner of the frames
	int pageNumber; 	//Number of the process page
	int frameUsed; 		//1 for used frames, 0 for empty frames
	int LRU; 			//Integer that represents the last time that the page was referenced
}Frame;

typedef struct requisicao{
	long mtype;
	int pid;
	int page;
}Requisicao;

typedef struct resp_a{
	long mtype;
	int ok;
}Resp;

Frame *TB_pages;
Requisicao teste;
Resp resposta;
int lru_cont = 1;
int *process_num = 0;

void resetFrameUsed() {
	int i;
	for (i = 0; i < NUMERO_FRAMES; ++i){
		TB_pages[i].frameUsed = 0;
	}
}

int aloca_pagina(Requisicao req) {
	int i;
	for (i = 0; i < NUMERO_FRAMES; i++) {
		if (TB_pages[i].frameUsed == 0) {
			TB_pages[i].processOwner = teste.pid;
			TB_pages[i].pageNumber = teste.page;
			TB_pages[i].frameUsed = 1;
			TB_pages[i].LRU = lru_cont;
			lru_cont++;
			(*process_num)++;
			break;
		}
	}
	return(i);
}

int main () {

	int idfila, idfila_p;
	int idshm_tb, idshm_np, cont;

	teste.mtype = 1;
	if ((idfila = msgget(KEY_MSG, IPC_CREAT | 0x1FF)) < 0) {
		printf("erro na criacao da fila\n");
		exit(1);
	}

	if((idshm_tb = shmget(KEY_MEM_TB, NUMERO_FRAMES * sizeof(Frame), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		TB_pages = (Frame *) shmat(idshm_tb, (void *)0, 0);
	}	

	if((idshm_np = shmget(KEY_MEM_NP, sizeof(int), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		process_num = (int *) shmat(idshm_np, (void *)0, 0);
	}

	resetFrameUsed();

	while(1) {
		if ((msgrcv(idfila, &teste, sizeof(Requisicao) - sizeof(long), 1, 0)) < 0) {
			printf("erro ao receber msg\n");
		} else {
			//printf("processo: %d\t pagina: %d\n", teste.pid, teste.page);
			printf("num process:%d\n", *process_num);
			if (*process_num >= MAX_OCUPACAO) {
				printf("Vou executar o substituifdor\n");
				kill(-1, SIGUSR1); //SIGUSR1
			}
			cont = aloca_pagina(teste);
			

			//TB_pages[0].pageNumber = teste.page;
			//TB_pages[0].processOwner = teste.pid;

			//printf("processo:%d page:%d, used:%d cont:%d\n", TB_pages[cont].processOwner, TB_pages[cont].pageNumber, TB_pages[cont].frameUsed, TB_pages[cont].LRU);

			if ((idfila_p = msgget(teste.pid, 0x1FF)) < 0) {
				printf("erro ao obter fila\n");
				exit(1);
			}
			resposta.mtype = 1;
			resposta.ok = 1;
			if ((msgsnd(idfila_p, &resposta, sizeof(int), 0)) < 0) {
				printf("erro ao enviar msg\n");
			}
		}

	}
	//msgctl(idfila, IPC_RMID, &teste);
	return 0;
}

/*

REMOVER FILAS
REMOVER MEM COMPARTILHADA


*/