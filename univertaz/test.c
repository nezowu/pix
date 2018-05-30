#define _GNU_SOURCE
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int HIDDEN;

typedef struct col {
	struct dirent **ar;
	int ar_len;
	int len;
} Col;

Col RAW;

struct dirent ** pwd(struct col *raw, char *path) {
        int i, j, count;
        int flag = HIDDEN;
	flag = 0;
        struct dirent **entry;
        int ar_len = scandir(path, &entry, 0, versionsort);
	raw->ar = entry;
        if(flag) {
                for(i = 2, j = 0; i < ar_len; i++) {
                        if (entry[i]->d_name[0] == '.')
                                continue;
                        raw->ar[j++] = entry[i];
                        count++;
                }
        }
        else {
                for(i = 2, j = 0; i < ar_len; i++) {
                        raw->ar[j++] = entry[i];
                        count++;
                }
        }
        raw->ar_len = count;
        raw->len = ar_len;
        return entry;
}
int main() {
	HIDDEN = 1;
	pwd(&RAW, "/home/nez/Projects/rain");
	for(int i = 0; i < RAW.ar_len; i++) {
		printf("%s\n", RAW.ar[i]->d_name);
	}
	printf("%zu %zu\n", sizeof(struct dirent), sizeof(struct dirent *));
//	for(int i = 0; i < RAW.ar_len; i++) {
//		free(RAW.ar[i]);
//	}
//	free(RAW.ar);
//					getcwd(buf, SIZ);
//                                      if(buf[1] != '\0')
//                                              strcat(buf, "/");
//                                      strcat(buf, RAW.ar[CURS]->d_name);
//                                      chdir(buf);}
