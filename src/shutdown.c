#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define NUMERO_FRAMES 30
#define KEY_MSG 0x1234
#define KEY_MEM 0x4321

typedef struct frame{
	int processOwner; 	//The process owner of the frames
	int pageNumber; 	//Number of the process page
	int frameUsed; 		//1 for used frames, 0 for empty frames
	int LRU; 			//Integer that represents the last time that the page was referenced
}Frame;

Frame *TB_pages;

int main(){

	int idshm;

	if((idshm = shmget(KEY_MEM, NUMERO_FRAMES * sizeof(Frame), 0x1FF)) < 0) {
		printf("Erro ao obter Mem\n");
	} else {
		if(shmctl(idshm, IPC_RMID, 0) == 0){

		}
	}

	return 0;
}