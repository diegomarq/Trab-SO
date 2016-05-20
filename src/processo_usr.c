#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_MSG 0x1234

pid_t getpid(void);

typedef struct requisicao{
	long mtype;
	int pid;
	int page;
}Requisicao;

typedef struct resp_a{
	long mtype;
	int ok;
}Resp;

Requisicao teste;
Resp resposta;

int main (int argc, char *argv[]) {
	int idfila = 0, idfila_p;
	char file_name[30];
	FILE *fp;

	if (argc < 1) {
		printf("Erro: usage: ./processo_usr file.txt\n");
	} else {
		teste.mtype = 1;
		teste.pid = getpid();

		strcpy(file_name, argv[1]);
		fp = fopen(file_name, "r");

		if ((idfila_p = msgget(teste.pid, IPC_CREAT | 0x1FF)) < 0) {
			printf("erro na criacao da fila\n");
			exit(1);
		}
		
		if ((idfila = msgget(KEY_MSG, 0x1FF)) < 0) {
			printf("erro na criacao da fila\n");
			exit(1);
		}

		while(fscanf(fp, "%d", &teste.page) == 1) {

			if ((msgsnd(idfila, &teste, sizeof(struct requisicao) - sizeof(long), 0)) < 0) {
				printf("erro ao enviar msg\n");
			}
			if ((msgrcv(idfila_p, &resposta, sizeof(int), 1, 0)) < 0) {
				printf("erro ao receber msg\n");
			}
			if(resposta.ok == 0){
				printf("Pagina não alocada\n");
				exit(1);
			} else if (resposta.ok == 1) {
				printf("Pagina alocada\n");
			}
		}
	}


	return 0;
}