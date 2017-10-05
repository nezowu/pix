#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>

int main() {
//	struct dirent *entry;
//	DIR *dir;
	char buf[128] = {0};
	printf("%s\n", getcwd(buf, 128));
	dirname(buf);
	printf("%s\n", buf);
//	printf("%s\n", dirname(buf));
//	dir = opendir(dirname(buf));
//	while((entry = readdir(dir)) != NULL) {
//		printf("%s\n", entry->d_name);
//	}
//	closedir(dir);
	return 0;
}
