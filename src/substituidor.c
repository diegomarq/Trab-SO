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

Frame *TB_pages;
int *process_num;

void desaloca_pagina() {
	int i;
	int lru, index;
	lru = 100000;
	for (i = 0; i < NUMERO_FRAMES; i++) {
		if (TB_pages[i].LRU < lru) {
			lru = TB_pages[i].LRU;
			index = i;
		}
	}
	TB_pages[index].processOwner = 0;
	TB_pages[index].pageNumber = 0;
	TB_pages[index].frameUsed = 0;
	TB_pages[index].LRU = 0;
	(*process_num)--;
	printf("Pagina substituida\n");
}

int main () {

	int idshm_np, idshm_tb;

	if((idshm_tb = shmget(KEY_MEM_TB, NUMERO_FRAMES * sizeof(Frame), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		TB_pages = (Frame *) shmat(idshm_tb, (void *)0, 0);
	}

	if((idshm_np = shmget(KEY_MEM_NP, sizeof(int), 0x1FF)) < 0) {
		printf("Erro ao obter Mem\n");
	} else {
		process_num = (int *) shmat(idshm_np, (void *)0, 0);
	}

	while(1) {
		signal(SIGUSR1, desaloca_pagina);
		while(*process_num >= MAX_OCUPACAO) {
			desaloca_pagina();
		}
	}
	return 0;
}