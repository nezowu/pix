#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define HASHSIZE 10
int hashlen;
uint32_t hash; /* последнее вычисленное значение хэш-функции */

typedef struct nm {
	uint32_t hash;
	char name[128];
	size_t len;
} Name[11];

uint32_t getHash(char * key){
	uint32_t i = 5381;
	while (*key++)
		i = ((i << 5) + i) ^ (unsigned int)*key; //(djb2a)
	return i;
//	uint64_t i = 0;
//	while(*key++) { //(sdbm)
//		i = *key + (i << 6) + (i << 16) - i;
//	}
//	return i;
}

Name Hash;
char * searchHash(char *);

int main() {
	memset(Hash, 0, sizeof(struct nm)*11);
	int i;
	hashlen = 0;
//	for(i = 0; i < 10; i++)
//		Hash[i] = (Name *)calloc(1, sizeof(struct name));
	while(1) {
		char buf[128];
		printf("%s", "Enter path to dir: ");
		scanf("%127[^\n]%*c", buf);
		searchHash(buf);	
		for(i = 0; i < hashlen; i++)
			printf("%u %s %zu\n", Hash[i].hash, Hash[i].name, Hash[i].len);
		if(buf[0] == 'q')
			break;
	}
}

char * searchHash(char *str) {
	int i;
	size_t len = strlen(str);
	Name tmp;
	uint32_t h = getHash(str);
	for(i = 0; i < hashlen; i++) {
		if(Hash[i].hash == h && Hash[i].len == len) {
			memcpy(Hash[i].name, str, strlen(str));
			memcpy(&tmp, &Hash[i], sizeof(struct nm));
			for(; i > 0; i--) {
				memset(&Hash[i], 0, sizeof(struct nm));
				memcpy(&Hash[i], &Hash[i-1], sizeof(struct nm));
			}
			memcpy(&Hash[0], &tmp, sizeof(struct nm));
			return Hash[0].name;
		}
	}
	if(hashlen < HASHSIZE)
		hashlen++;
	for(i = hashlen; i > 0; i--) {
		memset(&Hash[i], 0, sizeof(struct nm));
		memcpy(&Hash[i], &Hash[i-1], sizeof(struct nm));
	}
	memset(&Hash[0], 0, sizeof(struct nm));
	Hash[0].hash = h;
	memcpy(Hash[0].name, str, strlen(str));
	Hash[i].len = len;
	return Hash[0].name;
}
