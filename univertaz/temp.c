#include <time.h>
#include <utime.h>

time_t START = time(NULL);
atime(RAW.ar[CURS]->d_name);

void atime(char *path) {
	struct stat status;
	struct utimbuf buf;
	if(lstat(RAW.ar[CURS]->d_name, &status) == -1) { //if own == $user else
		perror("lstat:403");
		exit(EXIT_FAILURE);
	}
	buf.modtime = status.st_mtime;
	buf.actime = time(NULL);
	utime(RAW.ar[CURS]->d_name, &buf);
}
void pwd_prev(void) {
	struct dirent *entry_prev;
	int i;
	char buf[SIZ] = {0};
	getcwd(buf, SIZ);
	DIR *prev_col = opendir(dirname(buf));
	for(i = 0; readdir(prev_col) != NULL; i++) {
		PREV_LEN++;
	}
	rewinddir(prev_col);
