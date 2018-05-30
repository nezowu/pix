#include <stdio.h>
#include <string.h>
 
int checksum(char *s) {
    int c = 0;
 
    while(*s)
        c ^= *s++;
    return c;
}

int main() {
	char str[] = "hello world my good frends!";
	printf("%d\n", checksum(str));
}
