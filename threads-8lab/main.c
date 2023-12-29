#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int array[10];
int counter = 0;
pthread_mutex_t mtx; 

void *writeThread(void *arg) {
	while(1) {
		pthread_mutex_lock(&mtx);
		
		array[counter] = counter; // changed
		counter++;

		pthread_mutex_unlock(&mtx);

		sleep(2);

	}

	return NULL;
}

void *readThread(void *arg) {
	int tid = *(int*)arg;	
	while (1) {
		pthread_mutex_lock(&mtx);

		printf("Reader %d\ntid: %lx\n", tid, pthread_self());

		for (int i = 0; i < 10; i++) {
			printf("i = %i\n", array[i]);
		}
		
		pthread_mutex_unlock(&mtx);
		
		sleep(2);
	}
	
	return NULL;
}

int main() {
	pthread_mutex_init(&mtx, NULL);

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

	pthread_mutex_destroy(&mtx);

	return 0;
}
