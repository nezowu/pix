#include "dir.h"
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
#include <errno.h>
#include <stdint.h>
#define ERROR() {perror(NULL); goto END_PROG;} //нужно вести лог
#define END() goto END_PROG;

int CURS;
int OFFSET;
int MENULEN;
int ACCESS;
WINDOW *Prev, *Raw, *Next;
Col PREV, RAW, NEXT;
size_t bytesInPos(char *, int, int *);
void cadr_p(void);
void cadr(void);
void sig_handler(int);
void start_ncurses(void);
struct dirent ** pwd(struct col *, char *, bool);
void atime(char *);
void initHash(void);
uint32_t getHash(char *);
char * searchHash(char *, char *);
void reset(struct dirent **);
void reset_p(struct dirent **);
void reset_n(struct dirent **);
