/* Universidade de Brasilia
*  Departamento de Ciencia da Computacao
*  Sistemas Operacionais - 1/2016
*  Trabalho 1 - Gerenciador de Memoria
*  Diego Marques de Azevedo
*  Samuel Vinicius Vieira Pala
*/

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/sem.h>
#include <unistd.h>

#define NUMERO_FRAMES 10
#define MAX_OCUPACAO 9
#define	OCUPACAO_OK 8
#define NUM_PROC 5
#define KEY_MSG_REQ 0x1234
#define KEY_MSG_PRO 0x1235
#define KEY_MEM_TB 0x4321
#define KEY_MEM_NF 0x1100
#define KEY_MEM_SUB 0x1120
#define KEY_MEM_FAU 0x1130

pid_t getpid(void);
void p_sem();
void v_sem();
void init_sem();
int idsem;

struct sembuf operacao[1];

typedef struct frame{
	int processo; 	//Processo dono do frame
	int pagina; 	//numero da pagina
	int usado; 		//-1 para frames nao usados, 1 para usados
	int lru; 		//ultima referencia da pagina
}Frame;

typedef struct requisicao{
	long mtype;
	int pid;
	int pagina;
}Requisicao;

typedef struct resp_a{
	long mtype;
	int ok;
}Resp;

typedef struct pid_send{
	long mtype;
	int pid;
}Pid_s;

typedef struct page_f{
	int pid;
	int num;
}Page_F;

#endif
