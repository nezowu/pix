#define _XOPEN_SOURCE/* See feature_test_macros(7) */
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

int bytesInPos(char *, int);
int charwidth(char *, int *);

int main() {
	setlocale(LC_ALL, "");
	char buf[] = "azяю甲骨文周宣王𐤎𐤇"; //латиница, кирилица, китайская, финикийская
	int pos = 0;
	while(1) {
		printf("%s", "Enter: ");
		scanf("%d", &pos);
		printf("%d\n", bytesInPos(buf, pos));
	}
	return 0;
}

int bytesInPos(char *s, int pos) { //задача нати размер в байтах по позиции и корректно обрезать
	int size;
	int count = 0;
	int flag = 0;
	for (; *s; s++) {
		count++; //считаем байты
		flag++;
		if((*s & 0xC0) != 0x80) {
			flag = 0;
			pos--; //считаем позиции смволов одно и двух байтных
		}
		if(pos < 0) {
			if(flag == 1)
				count--;
			count--;
			break;
		}
		if(flag == 2) { //если мы досчитали до 3 байта, то пора проверить является ли символ широким?
			flag = 0;
			pos -= charwidth(s, &size); //если является то изменим позицию еще на единицу
			if(pos < 0) {
				count -= size;
				break;
			}
		}
	}
	return count;
}

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
