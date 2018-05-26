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

size_t bytesInPos(char *s, int pos, int *add_format) { //задача нати размер в байтах по позиции и корректно обрезать
	int post = pos;
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
				pos+=2;
				break;
			}
		}
	}
	*add_format = count - (post - pos); //смещение для форматной строки
	return count;
}
