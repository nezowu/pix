#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
	struct dirent **alphalist, **sortlist;
	int n, p;
	n = scandir("/home/nez/Projects/rain", &alphalist, 0, alphasort);
	if (n < 0)
		perror("scandir"), exit(EXIT_FAILURE);

	p = scandir("/home/nez/Projects/rain", &sortlist, 0, versionsort);
	if (p < 0)
		perror("scandir"), exit(EXIT_FAILURE);

	if( n == p ) {
		for(int i = 0; i < n; i++) {
			printf("%s %s\n", alphalist[i]->d_name, sortlist[i]->d_name);
			free(alphalist[i]);
			free(sortlist[i]);
		}
	}
	free(alphalist);
	free(sortlist);
}
