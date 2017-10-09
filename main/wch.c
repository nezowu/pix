#define _XOPEN_SOURCE
#include <wchar.h>
#include <stdlib.h>

int charwidth(char *s, int *size) {
	wchar_t wc;
	*size = 0;
	if((*(s+1) & 0xC0) != 0x80)
		*size = 3;
	else
		*size = 4;
	mbtowc(&wc, s-2, *size);
	if(wcwidth(wc) == 2)
		return 1; //значит символ двойной ширины
	return 0;
}
