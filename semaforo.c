/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

#include <errno.h>
#include "utilidades.h"

void p_sem() {
	// operacao[0].sem_num = 0;
	// operacao[0].sem_op = 0;
	// operacao[0].sem_flg = 0;
	operacao[0].sem_num = 0;
	operacao[0].sem_op = 1;
	operacao[0].sem_flg = 0;
	if ( semop(idsem, &operacao[0], 1) < 0)
		printf("erro no p=%d\n", errno);
}

void v_sem() {
	operacao[0].sem_num = 0;
	operacao[0].sem_op = -1;
	operacao[0].sem_flg = 0;
	if ( semop(idsem, &operacao[0], 1) < 0)
		printf("erro no p=%d\n", errno);
}

void init_sem() {
	// Verifica se existe
	idsem = semget(0x1101, 0, 0);
	if (idsem < 0) {
		// cria
		if ((idsem = semget(0x1101, 1, IPC_CREAT|0x1ff)) < 0) {
			printf("erro na criacao semaforo\n");
			exit(1);
		}
		if(semctl(idsem, 0, SETVAL, 1) < 0) {
			printf("erro\n");
			exit(1);
		}
	}	
}

