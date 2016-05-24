#ifndef SRC_H
#define SRC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

#define NUMERO_FRAMES 30
#define MAX_OCUPACAO 9
#define	OCUPACAO_OK 8
#define KEY_MSG 0x1234
#define KEY_MEM_TB 0x4321
#define KEY_MEM_NP 0x1100

typedef struct frame{
	int processOwner; 	//The process owner of the frames
	int pageNumber; 	//Number of the process page
	int frameUsed; 		//1 for used frames, 0 for empty frames
	int LRU; 			//Integer that represents the last time that the page was referenced
}Frame;

typedef struct requisicao{
	long mtype;
	int pid;
	int page;
}Requisicao;

typedef struct resp_a{
	long mtype;
	int ok;
}Resp;

#endif