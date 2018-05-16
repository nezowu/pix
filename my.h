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
#include <stdint.h>
#define ERROR() {perror(NULL); goto END_PROG;} //нужно вести лог
#define END() goto END_PROG;

time_t START;
int CURS;
int OFFSET;
int MENULEN;
int ACCESS;
int HIDDEN;
WINDOW *Prev, *Raw, *Next;

typedef struct col {
	struct dirent **ar;
	int ar_len;
	int len;
} Col;

Col PREV, RAW, NEXT;
int bytesInPos(char *, int, int *);
void cadr();
static void sig_handler(int);
void start_ncurses(void);
int pwd(struct col *, char *);
void atime(char *);
void initHash(void);
uint32_t getHash(char *);
char * searchHash(char *, char *);
