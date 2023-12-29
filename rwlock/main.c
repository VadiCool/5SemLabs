#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int array[10];
int counter = 0;

pthread_rwlock_t rwlock;

void *writeThread(void *arg) {
	while(1) {
		sleep(1);

		pthread_rwlock_wrlock(&rwlock);

		//sleep(1);

		array[counter % 10] = counter;
		counter++;

		pthread_rwlock_unlock(&rwlock);		
	}

	return NULL;
}

void *readThread(void *arg) {
	int tid = *(int*)arg;	
	while (1) {
		sleep(1);
		pthread_rwlock_rdlock(&rwlock);

		//sleep(1);

		printf("Reader %d\ntid: %lx\n", tid, pthread_self());

		for (int i = 0; i < 10; i++) {
			printf("i = %i\n", array[i]);
		}

		pthread_rwlock_unlock(&rwlock);
	}
	
	return NULL;
}

int main() {
	pthread_rwlock_init(&rwlock, NULL);

	pthread_t writer;
	pthread_t readers[10];

	pthread_create(&writer, NULL, writeThread, NULL);

	for (int i = 0; i < 10; i++) {
		pthread_create(&readers[i], NULL, readThread, (void *)&i);
	}

	pthread_join(writer, NULL);

	for (int i = 0; i < 10; i++) {
		pthread_join(readers[i], NULL);
	}

	pthread_rwlock_destroy(&rwlock);

	return 0;
}
