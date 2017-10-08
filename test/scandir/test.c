/*Вывод файлов в директории*/
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	struct dirent **namelist;
	int n;
	char * ar;
	if(argc == 2)
		ar = argv[1];
	else if(argc == 1)
		ar = ".";
	n = scandir(ar, &namelist, 0, alphasort);
	if (n < 0)
		perror("scandir");
	else {
		for(int i = 0; n != i; i++) {
			printf("%s %d %d\n", namelist[i]->d_name, namelist[i]->d_type, namelist[i]->d_reclen);
			free(namelist[i]);
		}
		free(namelist);
	}
	return 0;
}
