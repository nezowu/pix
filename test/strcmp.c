#include <stdio.h>
#include <stirng.h>
#define MAX_LEN 128
/* 1 связанные списки структур
 * 2 база с хешами
*/
typedef struct cmp {
	char name[MAX_LEN]; //path string
	int curs; //CURS index array
	int len_name; //maybe hash
	int len_items;
	int max_items; //if len == max_len then realloc; step 100;
	struct cmp *next;
} Cmp;

Cmp history;

void cmp(struct *cmp, char *str2) {
	int i;
	for(i = 0; i < cmp->len; i++) {
		if(strlen(str1) != strlen(str2))
			break;


int main() {
	char buf[128] = {0};
	printf("%s\n", "Enter your strings");
	scanf("%127[^\n]", buf);

