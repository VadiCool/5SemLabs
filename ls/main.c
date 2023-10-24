#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <stdlib.h>

#define DARK_BLUE \033[94m
#define GREEN     \033[92m
#define BLUE      \033[96m
#define NOCOLOR   \033[0m 

char *definePerm(char *pathToFile, char *permText) {
	
	struct stat fs;

	if (stat(pathToFile, &fs) == -1) exit(1);

	strncpy(permText, "----------", 10);

	if (S_ISREG(fs.st_mode)) permText[0] = '-';
	if (S_ISDIR(fs.st_mode)) permText[0] = 'd';
	if (S_ISLNK(fs.st_mode)) permText[0] = 'l';
	
	if (fs.st_mode & S_IRUSR) permText[1] = 'r';
	if (fs.st_mode & S_IWUSR) permText[2] = 'w';
	if (fs.st_mode & S_IXUSR) permText[3] = 'x';

	if (fs.st_mode & S_IRGRP) permText[4] = 'r';
	if (fs.st_mode & S_IWGRP) permText[5] = 'w';
	if (fs.st_mode & S_IXGRP) permText[6] = 'x';

	if (fs.st_mode & S_IROTH) permText[7] = 'r';
	if (fs.st_mode & S_IWOTH) permText[8] = 'w';
	if (fs.st_mode & S_IXOTH) permText[9] = 'x';

	return permText;
}

void lsL(char *fileName, char *pathToFile, char *permText) {
	definePerm(pathToFile, permText);
	struct stat fs;
	stat(pathToFile, &fs);

	char strTime[20];
	strftime(strTime, sizeof(strTime), "%b %d %H:%M", localtime(&fs.st_ctime));

	//printf("Total Second: %i\n", fs.st_blocks);

	if (permText[0] == 'd')
		printf("%s %2i %s %s %5i %s \033[94m%s\033[0m\n", permText, fs.st_nlink,
				getpwuid(fs.st_uid)->pw_name, getgrgid(fs.st_gid)->gr_name, fs.st_size, strTime, fileName);
	else if (permText[3] == 'x' || permText[6] == 'x' || permText[9] == 'x')
		printf("%s %2i %s %s %5i %s \033[92m%s\033[0m\n", permText, fs.st_nlink,
				getpwuid(fs.st_uid)->pw_name, getgrgid(fs.st_gid)->gr_name, fs.st_size, strTime, fileName);
	else if (permText[0] == 'l') printf("%s %2i %s %s %5i %s \033[96m%s\033[0m\n", permText, fs.st_nlink,
				getpwuid(fs.st_uid)->pw_name, getgrgid(fs.st_gid)->gr_name, fs.st_size, strTime, fileName);
	else printf("%s %2i %s %s %5i %s %s\n", permText, fs.st_nlink,
				getpwuid(fs.st_uid)->pw_name, getgrgid(fs.st_gid)->gr_name, fs.st_size, strTime, fileName);
}

int countTotal(DIR* d, char *path, int a_flag) {
	int total = 0;
	struct dirent *dir;

	while ((dir = readdir(d)) != NULL) {
		if (!a_flag) if (dir->d_name[0] == '.') continue;
		char *pathToFile = malloc(strlen(path) + strlen(dir->d_name) + 1);
		strcpy(pathToFile, path);
		strcat(pathToFile, dir->d_name);

		struct stat fs;
		stat(pathToFile, &fs);
		total += fs.st_blocks;
		
		//printf("Total First: %i\n %s\n", fs.st_blocks, dir->d_name);

		free(pathToFile);
	}
	return total/2; // convert bytes(512) in kbytes
}

int main(int argc, char** argv) {
	DIR *d;
	DIR *d2;
	struct dirent *dir;

	int res = 0;
	int a = 0;
	int l = 0;
	char *path; // path of dir

	char permText[] = "----------"; 
	while ((res = getopt(argc, argv, "al")) != -1) {
		switch(res) {
			case 'a':
				a = 1;	
				break;
			case 'l': 
				l = 1;
				break;			
			case '?':
				printf("Error");
				break;
		}
	}

	if (argc == 1 || argv[optind] == NULL) {
		d = opendir(".");
		d2 = opendir(".");
		path = (char *)malloc(3);
		strcpy(path, "./");
	}
	else if ((d = opendir(argv[optind])) && (d2 = opendir(argv[optind]))) {
		path = (char *)malloc(strlen(argv[optind] + 2));
		strcpy(path, argv[optind]);
		strcat(path, "/"); // path + / + file name. It needs for stat
	}
	else {
		printf("Error: can't find a directory\n");
		exit(-1);
	}
	
	if ((a && l) && d) {
		printf("total %i\n", countTotal(d2, path, a));
		while ((dir = readdir(d)) != NULL) {
			char *pathToFile = malloc(strlen(path) + strlen(dir->d_name) + 1);
			strcpy(pathToFile, path);
			strcat(pathToFile, dir->d_name);

			lsL(dir->d_name, pathToFile, permText);
			
			free(pathToFile);
		}
	}

	else if ((!a && l) && d) {
		printf("total %i\n", countTotal(d2, path, a));
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] == '.') continue;

			char *pathToFile = malloc(strlen(path) + strlen(dir->d_name) + 1);
			strcpy(pathToFile, path);
			strcat(pathToFile, dir->d_name);

			definePerm(pathToFile, permText);

			lsL(dir->d_name, pathToFile, permText);

			free(pathToFile);
		}
	}

	else if ((a && !l) && d) {
		while ((dir = readdir(d)) != NULL) {

			char *pathToFile = malloc(strlen(path) + strlen(dir->d_name) + 1);
			strcpy(pathToFile, path);
			strcat(pathToFile, dir->d_name);

			definePerm(pathToFile, permText);

			if (permText[0] == 'd') printf("\033[94m%s\033[0m  ", dir->d_name);
			else if (permText[3] == 'x' || permText[6] == 'x' || permText[9] == 'x') 
				printf("\033[92m%s\033[0m  ", dir->d_name);
			else if (permText[0] == 'l')
				printf("\033[94m%s\033[0m  ", dir->d_name);
			else printf("%s  ", dir->d_name);

			free(pathToFile);
		}
		printf("\n");
	}

	else {
		while ((dir = readdir(d)) != NULL) {
			
			if (dir->d_name[0] == '.') continue;

			// need for ../ . Without it will fileName. And its doesnt work with stat
			char *pathToFile = malloc(strlen(path) + strlen(dir->d_name) + 1); 
			strcpy(pathToFile, path);
			strcat(pathToFile, dir->d_name);
			
			definePerm(pathToFile, permText);	
			
			if (permText[0] == 'd') printf("\033[94m%s\033[0m  ", dir->d_name);
			else if (permText[3] == 'x' || permText[6] == 'x' || permText[9] == 'x') 
				printf("\033[92m%s\033[0m  ", dir->d_name);
			else if (permText[0] == 'l')
				printf("\033[94m%s\033[0m  ", dir->d_name);
			else printf("%s ", dir->d_name);

			free(pathToFile);
		}
		printf("\n");
	}

	free(path);
	closedir(d);

	return 0;
}
