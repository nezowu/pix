#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <locale.h>
#include <wchar.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <utime.h>
#include <errno.h>

int charwidth(char *, int *);
//time_t START;
//int CURS = 0;
//int OFFSET = 0;
//int MENULEN;
//int ACCESS;
//WINDOW *Prev, *Raw, *Next;
//
//typedef struct col {
//	struct dirent **ar;
//	int ar_len;
//} Col;
//
//Col PREV, RAW, NEXT;
//
//void cadr();
//static void sig_handler(int);
//void start_ncurses(void);
//int charwidth(char *, int *);
//int bytesInPos(char *, int);
//void pwd(struct col *);
