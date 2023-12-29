#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int main() {
	
	if (mkfifo("fifoFile", 0777) == -1) {
		if (errno != EEXIST) {
			printf("Cant create fifo\n");
			exit(EXIT_FAILURE);
		}
	}
	
	int id = fork();
	if (id == -1) {
		printf("Fork error\n");
		exit(EXIT_FAILURE);
	}
	if (id != 0) {

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

		int fd = open("fifoFile", O_WRONLY);
		if (fd == -1) {
			printf("Cant open file\n");
			exit(EXIT_FAILURE);
		}

		write(fd, str, 100);
		close(fd);

	}
	else {
		int fd = open("fifoFile", O_RDONLY);
		if (fd == -1) {
			printf("Cant open file\n");
			exit(EXIT_FAILURE);
		}

		char str[100];
		read(fd, str, 100);
		close(fd);
		printf("%s", str);
		
	}
	
	return 0;
}
