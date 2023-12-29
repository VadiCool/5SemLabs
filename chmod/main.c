#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

void parsePermChar(char *perm, mode_t *mode) {
	bool u = false;
       	bool g = false;
	bool o = false;
	
	if (perm[0] != 'u' && perm[0] != 'g' && perm[0] != 'o' && 
			perm[0] != '+' && perm[0] != '-') {
		printf("Invalid permission ugo\n");
		exit(EXIT_FAILURE);
	} else if (perm[0] == '+' || perm[0] == '-') {
		u = g = o = true;
	}

	int endUGO = 0; // end of user/group/other
	for(int i = 0; perm[i] != '\0'; i++) {
		if (perm[i] == 'u') u = true;
		else if (perm[i] == 'g') g = true;
		else if (perm[i] == 'o') o = true;
		else {
			endUGO = i;
			//printf("%i\n", endUGO);
			break;
		}
	}

	bool sign;
	if (perm[endUGO] == '-' || perm[endUGO] == '+') {
		if (perm[endUGO] == '-') sign = false;
		else sign = true;
		//printf("%c\n", perm[endUGO]);
	}
	else {
		//printf("%s %c %i\n", perm, perm[endUGO], endUGO);
		printf("Invalid permission sign\n");
		exit(EXIT_FAILURE);
	}

	bool x = false;
	bool r = false;
	bool w = false;
	
	for(int i = endUGO + 1; perm[i] != '\0'; i++) {
		//printf("%c\n", perm[i]);
		if (perm[i] != 'x' && perm[i] != 'r' && perm[i] != 'w') {
			//printf("%c %c %c %i\n", perm[0], perm[1], perm[2], i);
			//printf("%c %s\n", perm[i], perm);
			printf("Invalid permission perm\n");
			exit(EXIT_FAILURE);
		}
		if (perm[i] == 'x') x = true;
		else if (perm[i] == 'r') r = true;
		else if (perm[i] == 'w') w = true;
	}

	if (!(x || r || w)) {
		printf("No permission");
		exit(EXIT_FAILURE);
	}

	if (u) {
		if (sign == true) {
			if (x == true) *mode = *mode | S_IXUSR;
			if (r == true) *mode = *mode | S_IRUSR;
			if (w == true) *mode = *mode | S_IWUSR;
		}
		else {
			if (x == true) if (*mode & S_IXUSR) *mode -= S_IXUSR; // как то так & S_IXUSR) file.stat.mode - S_IXUSR
			if (r == true) if (*mode & S_IRUSR) *mode -= S_IRUSR;
			if (w == true) if (*mode & S_IWUSR) *mode -= S_IWUSR;
		}
	}
	if (g) {
		if (sign == true) {
			if (x == true) *mode = *mode | S_IXGRP;
			if (r == true) *mode = *mode | S_IRGRP;
			if (w == true) *mode = *mode | S_IWGRP;
		}
		else {
			if (x == true) if (*mode & S_IXGRP) *mode -= S_IXGRP; // как то так & S_IXUSR) file.stat.mode - S_IXUSR
			if (r == true) if (*mode & S_IRGRP) *mode -= S_IRGRP;
			if (w == true) if (*mode & S_IWGRP) *mode -= S_IWGRP;
		}
	}
	if (o) {
		if (sign == true) {
			if (x == true) *mode = *mode | S_IXOTH;
			if (r == true) *mode = *mode | S_IROTH;
			if (w == true) *mode = *mode | S_IWOTH;
		}
		else {
			if (x == true) if (*mode & S_IXOTH) *mode -= S_IXOTH; // как то так & S_IXUSR) file.stat.mode - S_IXUSR
			if (r == true) if (*mode & S_IROTH) *mode -= S_IROTH;
			if (w == true) if (*mode & S_IWOTH) *mode -= S_IWOTH;

		}
	}
}

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Too few arguments\n");
		return 1;
	}
	else if (argc > 3) {
		printf("Too much arguments\n");
		return 1;
	}

	struct stat statRes;
	if (stat(argv[2], &statRes) < 0) {
		printf("Stat error\n");
		return 1;
	}
	if (argc == 3) {
		errno = 0;
		char *end;
		strtol(argv[1], &end, 10); // check on errors
		//printf("%li %i\n", res, errno);
		if (*end == '\0') { // all symbols are numbers
			if (errno == ERANGE) {
				//printf("Over\n");
				exit(EXIT_FAILURE);
			}
			//printf("%o\n", statRes.st_mode);
			if (strtol(argv[1], NULL, 10) >= 0 && strtol(argv[1], NULL, 10) <=777) {
				//printf("%li\n", strtol(argv[1], NULL, 8));
				chmod(argv[2], strtol(argv[1], NULL, 8));
			}
			else {
				printf("Invalid permission\n");
				return 1;
			}
		}
		else {
			//printf("Was\n");
			stat(argv[2], &statRes);
			parsePermChar(argv[1], &statRes.st_mode);
			chmod(argv[2], statRes.st_mode);
		}
	}
	return 0;
}
