#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
           char *dirc, *basec, *bname, *dname;
           char *path = "/home/nez/Projects/rain";

           dirc = strdup(path);
	   printf("dirc=%s\n", dirc);
           basec = strdup(path);
//           dname = dirname(dirc);
//	   printf("dirc=%s\n", dirc);
//	   dirname(dirc);
	   printf("basec=%s\n", basec);
           bname = basename(basec);
	   printf("basec=%s\n", basec);
	   printf("bname=%s\n", bname);
	   dirname(basec);
	   printf("bname=%s\n", bname);
//           printf("dirname=%s, basename=%s\n", dname, bname);
	   printf("pid=%d ppid=%d\n", getpid(), getppid());
}
