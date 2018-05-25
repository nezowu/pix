#include <dirent.h>

typedef struct col {
	struct dirent **ar;
	int ar_len;
	int len;
} Col;

int HIDDEN;
