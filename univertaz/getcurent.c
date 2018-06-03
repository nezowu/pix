#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <dirent.h>


int main() {
//	struct dirent **entry;
//	int count = scandir(".", &entry, 0, alphasort);
//	unsigned short dig = 0;
//	char *lik;
//	char *current = getcwd(NULL, 256);
//	printf("%s\n", current);
//	lik = dirname(current);
//	printf("%s\n", current);
//	printf("%s\n", lik);
//	dig--;
//	printf("%u\n", dig);
//	free(current);
//	while(count > 0) {
//		count--;
//		printf("%s %d\n", entry[count]->d_name, entry[count]->d_reclen);
//	}
	char buf[256];
	char * current = getcwd(buf, 256);
	printf("%p\n", buf);
	printf("%s\n", current);
	char * dir = basename(buf);
	printf("%s\n", buf);
	printf("%s %p\n", dir, dir);
	printf("%s\n", current);
	printf("%p\n", buf);
}
