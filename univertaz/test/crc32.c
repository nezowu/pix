#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

unsigned long crc_table[256];

void crc_init()
{
        unsigned long t;
	int i;
        for(i=0;i<256;i++)
        {
                t=i;
                for(int j=8;j>0;j--)
                {
                        if(t&1)
                                t=(t>>1)^0xedb88320;
                        else
                                t>>=1;
                }
                crc_table[i] = t;
        }
}

unsigned long crc_cycle(unsigned long crc32, unsigned char * buf, ssize_t cnt)
{
        crc32=~crc32;
        for(ssize_t i=0;i<cnt;i++)
        {
                crc32=crc_table[(crc32^buf[i])&0xff]^(crc32>>8);
        }
        crc32=~crc32;
        return crc32;
}

int main() {
	char *buf = NULL;
	int l = 4;
	FILE *file;
	file = fopen("baobab.txt", "r");
	if(file = NULL) {
		fprintf(stderr, "%s\n", "Kajuk");
		return 1;
	}
        crc_init();
	int ok;
	size_t len = 0;
        unsigned long crc=0;
        for(int i=0;i<l;i++)
        {
                ok=getline(&buf, &len, file);
                crc=crc_cycle(crc,buf,3);
		printf("%08lx %s\n",crc, buf);
        }
}
