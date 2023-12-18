#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>


int id = 0;

void handler_sigint(int sig) {
	shmctl(id, IPC_RMID, NULL);
	printf("Handled\n");
	exit(0);
}

void handle_sigterm(int sig) {
	shmctl(id, IPC_RMID, NULL);
	printf("Handled\n");
	exit(0);
}

int main() {

	if (signal(SIGINT, handler_sigint) == SIG_ERR) {
		printf("Sigitn error\n");
		return 1;
	}

	struct sigaction sigTermAction;
	sigTermAction.sa_handler = handle_sigterm;
	if(sigaction(SIGTERM, &sigTermAction, NULL) == -1) {
		printf("Sigterm error\n");
		return 1;
	}
	

	//key_t key = ftok("file.txt", 1);
	
	key_t key = 100;
	if (-1 == key) {
		printf("Key error\n");
		return 1;
	}

	id = shmget(key, 1024, 0777 | IPC_CREAT | IPC_EXCL);

	if (-1 == id) {
		if (EEXIST == errno) {
			printf("Error: Shared memory with specified key exists\n");
			return 1;
		}
		else {
			printf("Shmget error\n");
			return 1;
		}
	}
	char *ptr = shmat(id, NULL, 0);

	printf("Data attached successfully\n");

	if (-1 == *((int*)ptr)) {
		printf("Shmat error\n");
		return 1;
	}
	
	while(1) {
		time_t mytime = time(NULL);
		char *curTime = ctime(&mytime);

		sprintf(ptr, "Time: %sPid: %i\n", curTime, getpid());

		// Your message
		strcat(ptr, "Hello world");
	}

	return 0;
}
