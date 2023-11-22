#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
void colorWord(char *str, char *wordColor) {
	// realloc to add color of text
	str = realloc(str, strlen(str) + 100);
	// overall capacity of current str
	int capacity = strlen(str) + 100;

	char *copyStr = (char*)malloc(strlen(str) + 10);
	strcpy(copyStr, str);

	int skip = 0;
	char *pos = NULL;
	int strCopyIdx = 0;
	int strCopySkip = 0;
	
	while ((pos = strstr(str + skip, wordColor)) != NULL) {

		int index = pos - str;
		
		str[index] = '\0';

		strcat(str, "\033[91m");
		strcat(str, wordColor);
		strcat(str, "\033[39m");

		char *strCopyPos = strstr(copyStr + strCopyIdx + strCopySkip, wordColor);

		strCopyIdx = strCopyPos - copyStr;
		strCopySkip = strlen(wordColor);
		
		strcat(str, copyStr + strCopyIdx + strlen(wordColor)); 

		int tempCapacity = capacity - strlen(str) - strlen("\033[91m") - strlen("\033[39m");
		if (tempCapacity < 50) {
			capacity += 100;
			str = realloc(str, capacity);
		}

		skip = index + strlen(wordColor) + strlen("\033[91m") + strlen("\033[91m");
	}
	
	if (strstr(str, wordColor)) {
		if (strstr(str, "\n")) printf("%s", str);
		else printf("%s\n", str);
	}

	free(copyStr);
	free(str);
}

int main(int argc, char **argv) {
	FILE *fp;
	char *buf = NULL;
	size_t size = 0;


	if (argc == 2) {
		while(getline(&buf, &size, stdin) != -1) {
			colorWord(buf, argv[1]);
			buf = NULL;
		}
		if (buf) free(buf); // if apeared error then buf != NULL
	} 
	if (argc == 3) {
		if ((fp = fopen(argv[argc - 1], "r"))) {
			while(getline(&buf, &size, fp) != -1) {
				colorWord(buf, argv[1]);
				buf = NULL;
			}
			fclose(fp);
			if (buf) free(buf); // if apeared error then buf != NULL
		}
		else {
			printf("Cant open file\n");
		}
	}
	return 0;
}
