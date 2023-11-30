#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void readAndPrint(FILE *stream, char *flags) {
	int n = 0;
	int b = 0;
	int E = 0;

	if (strstr(flags, "n")) n = 1;
	if (strstr(flags, "b")) b = 1;
	if (strstr(flags, "E")) E = 1;
	
	char *str = NULL;
	size_t strSize = 0; // size in bytes

	if (n && !b && !E) {
		size_t count = 1;
		while (getline(&str, &strSize, stream) != -1) {
			printf("%6li  %s", count, str);
			free(str);
			str = NULL;
			count++;
		}
		return;
	}
	else if (!n && b && !E) {
		size_t count = 1;
		while (getline(&str, &strSize, stream) != -1) {
			if (*str == '\n' || *str == '\0') {
				printf("\n");
			}
			else {
				printf("%6li  %s", count, str);

				free(str);
				str = NULL;
				count++;
			}
		}
		return;
	}
	else if (!n && !b && E) {
		while (getline(&str, &strSize, stream) != -1) {
			if (*str == '\n' || *str == '\0') {
				printf("$\n");
			}
			else {
				char tempStr[strlen(str)];
				strncpy(tempStr, str, strlen(str) - 1); // minus 1 coz \n
				strcat(tempStr, "\0");

				printf("%s$\n", tempStr);

				free(str);
				str = NULL;
			}
		}
		return;
	}
	else if (n && !b && E) {
	       	size_t count = 1;
		while (getline(&str, &strSize, stream) != -1) {
			if (*str == '\n' || *str == '\0') {
				printf("%6li  $\n", count);
				count++;
			}
			else {
				char tempStr[strlen(str)];
				strncpy(tempStr, str, strlen(str) - 1); // minus 1 coz \n
				strcat(tempStr, "\0");

				printf("%6li  %s$\n", count, tempStr);

				free(str);
				str = NULL;
				count++;
			}
		}
		return;
	}
 	else if (b && E) {
		size_t count = 1;
		while (getline(&str, &strSize, stream) != -1) {
			if (*str == '\n' || *str == '\0') {
				printf("$\n");
			}
			else {
				char tempStr[strlen(str)];
				strncpy(tempStr, str, strlen(str) - 1); // minus 1 coz \n
				strcat(tempStr, "\0");

				printf("%6li  %s$\n", count, tempStr);

				free(str);
				str = NULL;
				count++;
			}
		}
		return;
	}
	
}

int main(int argc, char **argv) {
	int res = 0;
	int n = 0;
	int b = 0;
	int E = 0;
	while((res = getopt(argc, argv, "nbE")) != -1) {
		switch(res) {
			case 'n':
				n = 1;
				break;
			case 'b':
				b = 1;
				break;
			case 'E':
				E = 1;
				break;
			case '?':
				exit(-1);
		}
	}
	if (argc == 1) {
		char *str = NULL;
		size_t n = 0;
		while (getline(&str, &n, stdin) != -1) {
			printf("%s", str);
			free(str);
			str = NULL;
		}
	}
	else if (argc >= 2) {
		FILE *fp = fopen(argv[optind], "r");
		if (!fp) {
			printf("Can't open file\n");
			exit(1);
		}
		if (n && !b && !E) {
			readAndPrint(fp, "n");
		}
		else if (b && !E) {
			readAndPrint(fp, "b");
		}
		else if (!n && !b && E) {
			readAndPrint(fp, "E");
		}
		else if (n && !b && E) {
		       readAndPrint(fp, "nE");
		}
 		else if (b && E) {
			readAndPrint(fp, "bE");
		}
		else if (!n && !b && !E) {
			size_t size = 0;
			char *buf;
			while (getline(&buf, &size, fp) != -1)
		       	printf("%s", buf);	
		}

	}

	return 0;
}

