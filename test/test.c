#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>

void printFileType(mode_t);
time_t start;

int main(int argc, char *argv[]) {
	start = time(NULL);
	struct stat buff;
	struct dirent *entry;
	char *path;
	DIR *dir;
	if(argc == 2)
		path = argv[1];
	else if(argc == 1)
		path = ".";
	else {
		perror("bad man(argc)");
		exit(EXIT_FAILURE);
	}
	dir = opendir(path);
	if(!dir) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	while((entry = readdir(dir)) != NULL) {
		if(!lstat(entry->d_name, &buff)) {
			if(errno) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		printFileType(buff.st_mode);

		printf(" %04o", buff.st_mode - (buff.st_mode / 010000) * 010000);
		printf(" %s:", (getpwuid(buff.st_uid))->pw_name);
		printf("%s", (getgrgid(buff.st_gid))->gr_name);
		printf(" %lu %s", buff.st_size, path);
		printf("%c%s ", '/', entry->d_name);
		printf("%d %d\n", buff.st_atime, start);
	}
	return 0;
}

void printFileType(mode_t st_mode) 
{                                   
    switch (st_mode & S_IFMT)
    {
        case S_IFDIR:  putchar('d'); break;
        case S_IFCHR:  putchar('c'); break;
        case S_IFBLK:  putchar('b'); break;
        case S_IFREG:  putchar('-'); break;
        case S_IFLNK:  putchar('l'); break;
        case S_IFSOCK: putchar('s'); break;
    }
    return;
}
