#define _XOPEN_SOURCE
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>

int countpos(wchar_t *, int);
int countutf(wchar_t *, int);

int main() {
	setlocale(LC_ALL, "");
//	char buf[] = "Prev_почемучка_window甲骨文周宣王.pdf";
	char buf[] = "azяю甲骨文周宣王";
	wchar_t utf[128];
	if(mbstowcs(utf, buf, strlen(buf)) == -1) {
		perror("mbstowcs");
		return -1;
	}
//	printf("%d %d %d\n", strlen(buf), wcslen(utf), wcswidth(utf, 10));
//	wprintf(L"%ls\n", utf);
	int i;
	for (i=0; i < wcslen(utf); i++) {
		wprintf(L"%lc %d %zu\n", utf[i], wcwidth(utf[i]), sizeof(utf[i]));
	}
	int len = countpos(utf, 10);
	int utflen_inpos = countutf(utf, 8);
	wprintf(L"%d %d", len, utflen_inpos);
	return 0;
}

int countpos(wchar_t *s, int ind) {
	int i;
	int pos = 0;
	for (i=0; i < ind; i++) 
		pos = pos + wcwidth(s[i]);
	return pos;
}

int countutf(wchar_t *s, int pos) {
	int len = 0, j = 0;
	int i;
	for (i=0; i < wcslen(s); i++) {
		len++;
		j += wcwidth(s[i]);
		if(j == pos)
			break;
		else if(j > pos) {
			len--;
			break;
		}
	}
	return len;
}
