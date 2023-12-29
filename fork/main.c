#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void myExit() {
	printf("Normal exit of function\n");
}

void handle_sigint(int sig) {
	printf("Handled sigint\n");
	exit(0);
}
void handle_sigterm(int sig) {
	printf("Handled sigterm\n");
	exit(0);
}

int main() {
	
	if (atexit(myExit)) printf("Atexit unsuccess\n");

	if (signal(SIGINT, handle_sigint) == SIG_ERR) printf("Error signal\n");

	struct sigaction sigTermAction;
	sigTermAction.sa_handler = handle_sigterm;
	if (sigaction(SIGTERM, &sigTermAction, NULL) == -1) printf("Error sigaction\n");

	pid_t pid = fork();
	
	if (pid == -1) {
		printf("Error fork");
		exit(-1);
	}
	else if (pid == 0) {
		sleep(3);
		printf("Child process id: %i\n", getpid());
	}
	else if (pid > 0) {
		wait(NULL);
		printf("Parant process id: %i\n", getpid());
	}
	
	printf("Hello world\n");

	return 0;
}
