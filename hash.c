#include <string.h>
#include <stdint.h>
#define HASHSIZE 255
static size_t hashlen;

typedef struct nm {
	uint32_t hash;
	char name[128];
	size_t len;
} Nm[256];

static Nm Hash;

void initHash(void) {
	memset(Hash, 0, sizeof(struct nm) * HASHSIZE);
	hashlen = 0;
}

uint32_t getHash(char * key){
	uint32_t i = 5381;
//	uint64_t i = 0;
	while (*key++)
		i = ((i << 5) + i) ^ (unsigned int)*key; //(djb2a 32)
//		i = *key + (i << 6) + (i << 16) - i; //(sdbm 64)
	return i;
}

char * searchHash(char *path, char * name) {
	int i;
	size_t len = strlen(name);
	Nm tmp;
	uint32_t hash = getHash(path);
	for(i = 0; i < hashlen; i++) {
		if(Hash[i].hash == hash) {
			if(len) {
				memset(Hash[i].name, 0, 128);
				memcpy(Hash[i].name, name, len);
			}
			memcpy(&tmp, &Hash[i], sizeof(struct nm));
			for(; i > 0; i--) {
				memset(&Hash[i], 0, sizeof(struct nm));
				memcpy(&Hash[i], &Hash[i-1], sizeof(struct nm));
			}
			memcpy(&Hash[0], &tmp, sizeof(struct nm));
			return Hash[0].name;
		}
	}
	if(!len)
		return NULL;
	if(hashlen < HASHSIZE)
		hashlen++;
	for(i = hashlen; i > 0; i--) {
		memset(&Hash[i], 0, sizeof(struct nm));
		memcpy(&Hash[i], &Hash[i-1], sizeof(struct nm));
	}
	memset(&Hash[0], 0, sizeof(struct nm));
	Hash[0].hash = hash;
	memcpy(Hash[0].name, name, len);
	Hash[i].len = len;
	return Hash[0].name;
}
