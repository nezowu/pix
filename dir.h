#include <dirent.h>
#include <stdbool.h>

typedef struct col {
	struct dirent **ar;
	int ar_len;
	int len;
} Col;
