/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

#include "utilidades.h"

Requisicao req;
Resp resposta;
Pid_s pid;
Frame *tabela;
Page_F *page_faults;
int lru_cont = 1;
int *num_pagina = 0;
int idfila, idfila_ok, idfila_pro, idshm_tb, idshm_nf, idshm_fault;

void zera_tabela() {
	int i;
	for (i = 0; i < NUMERO_FRAMES; i++){
		tabela[i].processo = -1;
		tabela[i].pagina = -1;
		tabela[i].usado = -1;
		tabela[i].lru = -1;
	}
}

void aloca_pagina() {
	int i, flag;

	flag = 0;

	for (i = 0; i < NUMERO_FRAMES; i++) {
		if ((tabela[i].processo == req.pid) && tabela[i].pagina == req.pagina) {
			tabela[i].lru = lru_cont;
			flag = 1;
		}
	}
	if(!flag) {
		for (i = 0; i < NUMERO_FRAMES; i++) {
			if(tabela[i].usado == 0) {
				tabela[i].processo = req.pid;
				tabela[i].pagina = req.pagina;
				tabela[i].usado = 1;
				tabela[i].lru = lru_cont;

				printf("Pagina alocada\n");

				(*num_pagina)++;
				break;
			}
		}
		flag = 0;
		for (i = 0; i < NUM_PROC; i++) {
			if(page_faults[i].pid == req.pid) {
				page_faults[i].num++;
				flag = 1;
				break;
			}
		}
		if(!flag) {
			for (i = 0; i < NUM_PROC; i++) {
			if(page_faults[i].pid == 0) {
				page_faults[i].pid = req.pid;
				page_faults[i].num++;
				break;
			}
		}
		}
	}
}

void trata(){

	printf("Encerrando...\n");
	//Remove fila de requisicoes
	msgctl(idfila, IPC_RMID, NULL);

	//Remove memoria compartilhada da tabela
	shmdt(tabela);
	shmctl(idshm_tb, IPC_RMID, NULL);

	//Remove memoria compartilhada da quantidade de frames
	shmdt(num_pagina);
	shmctl(idshm_nf, IPC_RMID, NULL);

	//Remove memoria compartilhada da quantidade de page faults
	shmdt(page_faults);
	shmctl(idshm_fault, IPC_RMID, NULL);

	//Remove semaforo
	semctl(idsem, 0, IPC_RMID);

	exit(1);
}

int main() {

	signal(SIGUSR1, trata);
	init_sem();

	//Zera a tabela (seta todos os valores como -1)
	//zera_tabela();

	//Cria fila de msg para as requisicoes
	if ((idfila = msgget(KEY_MSG_REQ, IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro na criacao da fila\n");
		exit(1);
	}

	//Cria fila de msg para pids dos processos
	if ((idfila_pro = msgget(KEY_MSG_PRO, IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro na criacao da fila\n");
		exit(1);
	}

	//Envia msg de pid
	pid.mtype = 1;
	pid.pid = getpid();
	if ((msgsnd(idfila_pro, &pid, sizeof(int), 0)) < 0) {
		printf("erro ao enviar msg\n");
	}

	//Cria mem compartilhada com a tabela de paginas
	if((idshm_tb = shmget(KEY_MEM_TB, NUMERO_FRAMES * sizeof(Frame), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		tabela = (Frame *) shmat(idshm_tb, (void *)0, 0);
	}

	//Cria mem compartilhada para quantidade de frames
	if((idshm_nf = shmget(KEY_MEM_NF, sizeof(int), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		num_pagina = (int *) shmat(idshm_nf, (void *)0, 0);
		*num_pagina = 0;
	}

	//Cria mem compartilhada para quantidade de page faults
	if((idshm_fault = shmget(KEY_MEM_FAU, NUM_PROC * sizeof(Page_F), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		page_faults = (Page_F *) shmat(idshm_fault, (void *)0, 0);
	}

	while(1) {
		//Recebe a mensagem de alocacao
		if ((msgrcv(idfila, &req, sizeof(Requisicao) - sizeof(long), 1, 0)) < 0) {
			printf("erro ao receber msg\n");
		}

		//Pega a fila de confirmacao de alocacao do processo corrente
		if ((idfila_ok = msgget(req.pid, 0x1FF)) < 0) {
				printf("erro ao obter fila\n");
				exit(1);
		}
		while(*num_pagina >= MAX_OCUPACAO) {
		}
			p_sem();
			aloca_pagina();
			v_sem();
			lru_cont++;


		resposta.mtype = 1;
		resposta.ok = 1;
		//Envia msg de confirmacao de alocacao
		if ((msgsnd(idfila_ok, &resposta, sizeof(int), 0)) < 0) {
			printf("erro ao enviar msg\n");
		}
		//printf("MSG Recebida: pro:%d pg:%d\n", req.pid, req.pagina);
	}

	return 0;
}
