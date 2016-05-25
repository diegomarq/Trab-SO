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
int *subs_exec = 0;
int *num_pagina;
int idshm_tb, idshm_nf, idfila_pro, idshm_sub;

void desaloca_pagina() {
	int i, j, indice, usado;
	
	
	i = 0;
	while(tabela[i].usado == 0) {
		i++;
	}

	usado = tabela[i].lru;
	indice = i;

	for (j = i; j < NUMERO_FRAMES; j++) {
		if ((tabela[j].usado == 1) && tabela[j].lru < usado) {
			usado = tabela[j].lru;
			indice = j;
		}
	}
	tabela[indice].processo = 0;
	tabela[indice].pagina = 0;
	tabela[indice].usado = 0;
	tabela[indice].lru = 0;
	printf("Pagina removida\n");
	(*num_pagina)--;
}

void trata() {
	printf("Encerrando...\n");

	//Remove memoria compartilhada da quantidade de frames
	shmdt(num_pagina);
	shmctl(idshm_nf, IPC_RMID, NULL);

	//Remove memoria compartilhada da tabela
	shmdt(tabela);

	//Remove memoria compartilhada de subs exec
	shmdt(subs_exec);
	shmctl(idshm_sub, IPC_RMID, NULL);

	exit(1);
}

int main() {

	signal(SIGUSR1, trata);

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

	//Obtem mem compartilhada com a tabela de paginas
	if((idshm_tb = shmget(KEY_MEM_TB, NUMERO_FRAMES * sizeof(Frame), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		tabela = (Frame *) shmat(idshm_tb, (void *)0, 0);
	}

	//Obtem mem compartilhada para quantidade de frames
	if((idshm_nf = shmget(KEY_MEM_NF, sizeof(int), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		num_pagina = (int *) shmat(idshm_nf, (void *)0, 0);
	}

	//Cria mem compartilhada para execuções do processo de substituicao
	if((idshm_sub = shmget(KEY_MEM_SUB, sizeof(int), IPC_CREAT | 0x1FF)) < 0) {
		printf("Erro ao criar Mem\n");
	} else {
		subs_exec = (int *) shmat(idshm_sub, (void *)0, 0);
		*subs_exec = 0;
	}

	while(1) {
		while(*num_pagina >= MAX_OCUPACAO && *num_pagina > OCUPACAO_OK) {
			(*subs_exec)++;
			desaloca_pagina();
		}

	}

	return 0;
}
