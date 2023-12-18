#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main() {
	
	//key_t key = ftok("file.txt", 1);
	key_t key = 100;

	if (-1 == key) {
		printf("Ftok error\n");
		return 1;
	}

	int id = shmget(key, 1024, 0777);

	if (id == -1) {
		printf("Shmget error\n");
		return 1;
	}

	char *ptr = shmat(id, NULL, 0);

	if (-1 == *((int*)ptr)) {
		printf("Shmat error\n");
		return 1;
	}

	printf("%s\n", ptr);	

	if (-1 == shmdt(ptr)) {
		printf("Shmdt error\n");
		return 1;
	}

	return 0;
}
