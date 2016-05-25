/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>


// typedef struct _sembuf {
// 	ushort sem_num;
// 	short sem_op;
// 	short sem_flg;
// }sembuf;

struct sembuf operacao[1];
int idsem;
int *psem;

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
	idsem = semget(0x1223, 0, 0);
	if (idsem < 0) {
		// cria
		if ((idsem = semget(0x1223, 1, IPC_CREAT|0x1ff)) < 0) {
			printf("erro na criacao semaforo\n");
			exit(1);
		}
		if(semctl(idsem, 0, SETVAL, 1) < 0) {
			printf("erro\n");
			exit(1);
		}
	}	
}

int main() {
	//int pid;
	//int *psem;
	init_sem();
		
	p_sem();
	printf("pai - obtive o semaforo, vou dormir\n");
	sleep(15);
	printf("pai - dormi\n");
	v_sem();
	semctl(idsem,1,IPC_RMID);

	// wait(&estado);

	// while(1){};

	exit (0);

}
