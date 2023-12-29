#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
	int fd[2];

	if(pipe(fd) == -1) {
		printf("Error\n");
		return -1;
	}

	int id = fork();
	if (id == -1) {
		printf("Fork error\n");
		return -1;
	}
	if (id != 0) {
		close(fd[0]);

		char str[100] = "";
		strcat(str, "Hello word!\n");
		strcat(str, "id: ");

		char strId[10] = "";
		sprintf(strId, "%i", id);

		strcat(str, strId);
		strcat(str, "\n");

		time_t mytime = time(NULL);
		char *curTime = ctime(&mytime);
		strcat(str, curTime);

		sleep(5);

		write(fd[1], str, 100);
		close(fd[1]);

	} else {
		close(fd[1]);
		char str[100] = "";
		read(fd[0], str, 100);
		close(fd[0]);
		printf("%s", str);
	}



	return 0;
}
