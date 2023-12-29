#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int array[10];
int counter = 0;
pthread_mutex_t mtx;
pthread_cond_t cond_var;

void *writeThread(void *arg) {
	while(1) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		//pthread_cond_wait(&cond_var, &mtx);

		array[counter % 10] = counter; // changed
		counter++;
	
		//sleep(2);

		pthread_cond_broadcast(&cond_var);
		pthread_mutex_unlock(&mtx);
		//pthread_cond_signal(&cond_var);
	}

	return NULL;
}

void *readThread(void *arg) {
	int tid = *(int*)arg;	
	while (1) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		pthread_cond_wait(&cond_var, &mtx);

		printf("Reader %d\ntid: %lx\n", tid, pthread_self());

		for (int i = 0; i < 10; i++) {
			printf("i = %i\n", array[i]);
		}

		//pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mtx);
		
	}
	
	return NULL;
}

int main() {
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond_var, NULL);

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
	pthread_cond_destroy(&cond_var);

	return 0;
}
