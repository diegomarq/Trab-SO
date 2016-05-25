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
int idfila, idfila_ok, idfila_pro;

void trata() {
	printf("Tratei FUI\n");

	//Remove fila de requisicoes
	msgctl(idfila, IPC_RMID, NULL);

	//Remove fila de confirmacao de alocacao
	msgctl(idfila_ok, IPC_RMID, NULL);

	exit(1);
}

int main (int argc, char *argv[]) {

	signal(SIGUSR1, trata);

	char file_name[30];
	FILE *fp;

	if (argc < 2) {
		printf("Erro: usage: ./processo_usr test/<file.txt>\n");
		exit(1);
	} else {
		req.mtype = 1;
		req.pid = getpid();
		strcpy(file_name, argv[1]);
		fp = fopen(file_name, "r");

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

		//Fila de mensagens para requisicao ao alocador
		if ((idfila = msgget(KEY_MSG_REQ, IPC_CREAT | 0x1FF)) < 0) {
			printf("Erro na criacao da fila\n");
			exit(1);
		}
		//Fila de confirmacao de alocacao
		if ((idfila_ok = msgget(req.pid, IPC_CREAT | 0x1FF)) < 0) {
			printf("erro na criacao da fila\n");
			exit(1);
		}

		//Leitura do arquivo com as paginas para alocacao
		while(fscanf(fp, "%d", &req.pagina) == 1) {
			//Envia a requisicao (pid e pagina a ser alocada)
			if ((msgsnd(idfila, &req, sizeof(struct requisicao) - sizeof(long), 0)) < 0) {
				printf("erro ao enviar msg\n");
			}
			//Recebe confirmacao de alocacao
			if ((msgrcv(idfila_ok, &resposta, sizeof(int), 1, 0)) < 0) {
				printf("erro ao receber msg\n");
			}
			// if(resposta.ok == 0){
			// 	printf("Pagina não alocada\n");
			// 	exit(1);
			// } else if (resposta.ok == 1) {
			// 	printf("Pagina alocada\n");
			// }
			// printf("MSG enviada pro:%d pg:%d\n", req.pid, req.pagina);
		}
	}
	//Remove fila de confirmacao de alocacao
	msgctl(idfila_ok, IPC_RMID, NULL);
	
	return 0;
}
