/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

#include "utilidades.h"

Frame *tabela;
Pid_s pid;
Page_F *page_faults;
int *subs_exec;

void printa_tabela() {
	int i;
	printf("------------------------------------\n");
	printf("|Processo | Pagina | Usado |  LRU  |\n");
	printf("------------------------------------\n");
	for (i = 0; i < NUMERO_FRAMES; i++) {
		if(tabela[i].processo == 0) {
			printf("|  Livre  | Livre  | Livre | Livre |\n");
		} else {
			printf("|   %d  |    %d   |   %d   |   %d   |\n", tabela[i].processo, tabela[i].pagina, tabela[i].usado, tabela[i].lru);
		}
	}
}

int main() {

	int idshm_tb, idfila_pro, idshm_sub, idshm_fault;
	int i = 0, total_fault = 0;

	//Obtem mem compartilhada com a tabela de paginas
	if((idshm_tb = shmget(KEY_MEM_TB, NUMERO_FRAMES * sizeof(Frame), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		tabela = (Frame *) shmat(idshm_tb, (void *)0, 0);
	}

	//Cria fila de msg para pids dos processos
	if ((idfila_pro = msgget(KEY_MSG_PRO, IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro na criacao da fila\n");
		exit(1);
	}

	//Obtem mem compartilhada para execuções do processo de substituicao
	if((idshm_sub = shmget(KEY_MEM_SUB, sizeof(int), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		subs_exec = (int *) shmat(idshm_sub, (void *)0, 0);
	}

	//Cria mem compartilhada para quantidade de page faults
	if((idshm_fault = shmget(KEY_MEM_FAU, NUM_PROC * sizeof(Page_F), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		page_faults = (Page_F *) shmat(idshm_fault, (void *)0, 0);
	}
	for (i = 0; i < NUM_PROC; i++) {
		total_fault+= page_faults[i].num;
		if (page_faults[i].pid > 0) {
			printf("Numero de page_faults do processo %d: %d\n", page_faults[i].pid, page_faults[i].num);
		}
	}
	printf("Numero de page faults total: %d\n", total_fault);
	printf("Numero de execucoes do processo de substituicao: %d\n", *subs_exec);
	printa_tabela();

	//Recebe a mensagem de pid
	while((msgrcv(idfila_pro, &pid, sizeof(Requisicao) - sizeof(long), 1, IPC_NOWAIT)) > 0){
		kill(pid.pid, SIGUSR1);
	}
	

	//Remove memoria compartilhada da tabela
	shmdt(tabela);
	shmctl(idshm_tb, IPC_RMID, NULL);

	//Remove semaforo tabela
	semctl(idsem,1,IPC_RMID);

	//Remove fila de pid
	msgctl(idfila_pro, IPC_RMID, NULL);

	//Remove memoria compartilhada de subs exec
	shmdt(subs_exec);

	//Remove memoria compartilhada de page faults
	shmdt(page_faults);

	return 0;
}
