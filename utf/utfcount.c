#include <stdio.h>
#include <locale.h>
#include <string.h>
size_t utflen(char *);
size_t utfcount(char *, int);
size_t utfpos(char *);
size_t countpos(char *s, int);

int main() {
	setlocale(LC_ALL, "");
//	char buf[] = "Prev_почемучка_window甲骨文周宣王.pdf";
	char buf[] = "azяю甲骨文周宣王";
	char dst[32];
	memset(dst, 0, 32);
	printf("%s\n", buf);
	printf("strlen: %zu utflen: %zu utfcount: %zu\n", strlen(buf), utflen(buf), utfcount(buf, 3));
	printf("utfpos: %zu countpos: %zu\n", utfpos(buf), countpos(buf, 3));
	memcpy(dst, buf, utfcount(buf, 3));
	printf("%s\n", dst);
	return 0;
}
size_t utflen(char *s) {
        size_t len = 0;
        for (; *s; s++) {
                if ((*s & 0xC0) != 0x80)
                        len++;
        }
        return len;
}

size_t utfcount(char *s, int ind) {
        size_t len = 0;
	size_t glob = 0;
        for (; *s; s++) {
		glob++;
                if ((*s & 0xC0) != 0x80)
                        len++;
		if(len == ind)
			break;
        }
        return glob-1;
}

size_t utfpos(char *s) {
        size_t len = 0;
	size_t flag = 0;
        for (; *s; s++) {
		flag++;
                if ((*s & 0xC0) != 0x80) {
                        len++;
			flag = 0;
		}
		if(flag == 2)
			len++;
        }
        return len;
}

size_t countpos(char *s, int ind) {
        size_t len = 0;
	size_t glob = 0;
	size_t flag = 0;
        for (; *s; s++) {
		glob++;
                if ((*s & 0xC0) != 0x80) {
                        len++;
			if(len == ind)
				break;
		}
	}
        return glob-1;
}
