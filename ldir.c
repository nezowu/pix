#define _GNU_SOURCE
#include "dir.h"
#include <stdlib.h>
#include <string.h>

struct dirent ** pwd(struct col *raw, char *path, bool flag) {
	int i, j, ar_len;
	int offset = 0, count = 0;
	struct dirent **entry;
	ar_len = scandir(path, &entry, 0, versionsort);
//	if(ar_len > 2) {
	if(flag) {
		for(i = 2; i < ar_len; i++) {
			if(entry[i]->d_name[0] != '.') {
				count++;
				if(entry[i]->d_type == DT_DIR)
					offset++;
			}
		}
		raw->ar = (struct dirent **)malloc(count * sizeof(struct dirent *));
		for(i = 2, j = 0; i < ar_len; i++) {
			if(entry[i]->d_name[0] != '.') {
				if(entry[i]->d_type == DT_DIR)
					raw->ar[j++] = entry[i];
				else
					raw->ar[offset++] = entry[i];
			}
		}
	}
	else {
		for(i = 2; i < ar_len; i++) {
			if(entry[i]->d_type == DT_DIR)
				offset++;
		}
		count = ar_len - 2;
		raw->ar = (struct dirent **)malloc(count * sizeof(struct dirent *));
		for(i = 2, j = 0; i < ar_len; i++) {
			if(entry[i]->d_type == DT_DIR)
				raw->ar[j++] = entry[i];
			else
				raw->ar[offset++] = entry[i];
		}
	}
//	}
//	else {
//		raw->ar = (struct dirent **)malloc(sizeof(struct dirent *));
//		raw->ar[0] = (struct dirent *)malloc(sizeof(struct dirent));
//		memcpy(raw->ar[0]->d_name, "Empty", 6);
//		raw->ar[0]->d_type = DT_UNKNOWN;
//		offset++;
//	}
	raw->ar_len = offset;
	raw->len = ar_len;
	return entry;
}
