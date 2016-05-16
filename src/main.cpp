#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <string.h>

// DEFINES

#define NUMERO_PROCESSOS 1
#define NUMERO_FRAMES 10
#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8

using namespace std;
 
struct virtual_mem{
    int processo;
    int pagina;
    int used;
};

struct mensagem {
    long pid;
    char msg[30];
};

int pid, idfila; fd[2], estado;
 
int page_foult[NUMERO_PROCESSOS]; // lembrar de zerar
int pages = 0;
int substituicoes = 0;
 
int referencia_pagina() {
    if(pages < MAX_OCUPACAO) {
        //percorre a tabela de paginas
        //encontra uma vazia
        //mapeia a page
 
    } else {
        libera_pagina();
    }
}
 
int main () {
    mensagem mensagem_env;
    mensagem mensagem_rec;

    if(id_fila = msgget(0x1223, IPC_CREATE|0x1ff) < 0) {
        printf("error\n");
        exit(1);
    }

    pid = fork();
    if(pid == 0) {
        mensagem_env.pid = getpid();
        strcpy(mensagem_env, "teste msg");
        msgsnd(idfila, &mensagem_env, sizeof(mensagem_env), 0);
        exit(0);
    }

    return 0;
}