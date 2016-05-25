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
#include <wait.h>

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
	if (semop(idsem, &operacao[0], 1) < 0)
		printf("erro no p=%d\n", errno);	
}

void v_sem() {
	operacao[0].sem_num = 0;
	operacao[0].sem_op = -1;
	operacao[0].sem_flg = 0;
	if ( semop(idsem, &operacao[0], 1) < 0)
		printf("erro no p=%d\n", errno);
}

int main() {
	// int pid;
	//int *psem;
	
	// if ((idsem = semget(0x1223, 1, 0x1ff)) < 0) {
	// 	printf("erro na obtecao idsem\n");
	// 	exit(1);
	// }

	idsem = semget(0x1223, 0, 0);	
	while(1) {
		p_sem();
		printf("filho - obtive o semaforo, vou dormir\n");
		sleep(1);
		printf("filho - dormi\n");
		v_sem();
	}
	
	exit (0);

}
