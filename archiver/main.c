#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

struct ArchData {
	size_t size;
	char name[2000];
	mode_t mode;
};

void archiveFile(char *fileName, char *archName, int archFD) {
	int fileFD = open(fileName, O_RDWR, 0777); // fd of fileName

	if (fileFD == -1) {
		printf("File open error\n");
		exit(EXIT_FAILURE);
	}
	
	// save name of file
	struct ArchData arch;
	
	strcpy(arch.name, fileName);

	// save mode
	struct stat statFile;
	if (lstat(fileName, &statFile) == -1) {
		printf("Cant open stat file");
		exit(EXIT_FAILURE);
	}
	arch.mode = statFile.st_mode;
	
	// save size of file
	if ((arch.size = lseek(fileFD, 0, SEEK_END)) == -1) {
		printf("File lseek error\n");
		exit(EXIT_FAILURE);	
	}

	// return fileFD to beginning
	lseek(fileFD, 0, SEEK_SET);
	
	if (write(archFD, &arch, sizeof(struct ArchData)) == -1) printf("WRITE1\n");
	
	char buf[arch.size];
	if (read(fileFD, &buf, arch.size) == -1) {
		printf("Read error\n");
		exit(EXIT_FAILURE);
	}
	if (write(archFD, &buf, arch.size) == -1) {
		printf("Write error\n");
		exit(EXIT_FAILURE);
	}

	remove(fileName);
	
	close(fileFD);
}

void extractFile(char *archiveName, int archFD) {
	
	struct ArchData arch;

	if (archFD == -1) {
		printf("Cant open archive\n");
		exit(EXIT_FAILURE);
	}

	int readCount = -1; // count of read symbols
	while (1) {
		if ((readCount = read(archFD, &arch, sizeof(struct ArchData))) == -1) {
			printf("Cant read from archive\n");
			exit(EXIT_FAILURE);
		}

		if (readCount == 0) { // end of file
			remove(archiveName);
			exit(0);
		}

		//printf("%li\n", arch.size);
		//printf("%s\n", arch.name);
		//printf("%i\n", arch.mode);
		
		int fileFD = open(arch.name, O_CREAT | O_WRONLY, 0777);

		if (fileFD == -1) {
			printf("Cant create file\n");
			exit(EXIT_FAILURE);
		}
		char buf[arch.size];
		if (read(archFD, &buf, arch.size) == -1) {
			printf("Cant read data from archive\n");
			exit(EXIT_FAILURE);
		}
		if (write(fileFD, &buf, arch.size) == -1) {
			printf("Cant write data to file\n");
			exit(EXIT_FAILURE);
		}
		

		chmod(arch.name, arch.mode);
	}

}

void status(struct ArchData *arch, int archFD) {
	int readCount = -1; // count of read symbols
	while (1) {
		if ((readCount = read(archFD, arch, sizeof(struct ArchData))) == -1) {
			printf("Cant read from archive\n");
			exit(EXIT_FAILURE);
		}

		if (readCount == 0) exit(0);

		printf("File: %s\n", arch->name);
		printf("Size: %li\n", arch->size);
		printf("Mode: %o\n", arch->mode & 0777);
		printf("\n");
		
		char buf[arch->size];
		if (read(archFD, &buf, arch->size) == -1) {
			printf("Read error\n");
			exit(EXIT_FAILURE);
		}
	}
}
int main(int argc, char **argv) {
	if (argc >= 3) {
		if (strcmp(argv[2], "-i") == 0 || strcmp(argv[2], "--input") == 0) {
			int archFD = open(argv[1], O_CREAT | O_RDWR, 0777);
			if (archFD == -1) {
				printf("Cant create archive\n");
				exit(EXIT_FAILURE);
			}

		       	int count = 3;
			while(count != argc) {
				archiveFile(argv[count], argv[1], archFD);
				count++;
			}
			close(archFD);
		}
		else if (strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "--extract") == 0) {
			int archFD = open(argv[1], O_RDWR, 0777);
			if (archFD == -1) {
				printf("Cant create archive\n");
				exit(EXIT_FAILURE);
			}

			extractFile(argv[1], archFD);
			printf("e\n");
		}
		else if (strcmp(argv[2], "-s") == 0 || strcmp(argv[2], "--stat") == 0) {
			struct ArchData arch;
			int archFD = open(argv[1], O_RDWR, 0777);
			if (archFD == -1) {
				printf("Cant open archive\n");
				exit(EXIT_FAILURE);
			}
			status(&arch, archFD);
		}
		else {
			printf("Wrong input\n");
			exit(EXIT_FAILURE);
		}		
	}
	else if ((2 == argc) && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
			printf("./archiver arch_name -i(--input) file1 ... - archive files\n");
			printf("./archiver arch_name -e(--extract) - extract files from archive\n");
			printf("./archiver arch_name -s(--stat) - show stat of files in archive\n");
			printf("./archiver -h(--help) - show instruction\n");
	}
	else {
		printf("Wrong input\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
