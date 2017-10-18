#include <inttypes.h>	/* intptr_t, PRIxPTR */
#include <search.h>	/* hcreate(), hsearch() */
#include <stdio.h>	/* perror(), printf() */
#include <stdlib.h>	/* exit() */
 
void fail(char *message) {
	perror(message);
	exit(1);
}
 
/*
 * Must hcreate() the hash table before calling fetch() or store().
 *
 * Because p->data is a pointer, fetch() and store() cast between
 * void * and intptr_t.
 */
 
/* Fetch value from the hash table. */
int fetch(const char *key, intptr_t *value) {
	ENTRY e = {key: (char *)key}, *p;
	p = hsearch(e, FIND);
	if (p) {
		*value = (intptr_t)p->data;
		return 1;
	} else
		return 0;
}
 
/* Store key-value pair into the hash table. */
void store(const char *key, intptr_t value) {
	/*
	 * hsearch() may insert a new entry or find an existing entry
	 * with the same key. hsearch() ignores e.data if it finds an
	 * existing entry. We must call hsearch(), then set p->data.
	 */
	ENTRY e = {key: (char *)key}, *p;
	p = hsearch(e, ENTER);
	if (p == NULL)
		fail("hsearch");
	p->data = (void *)value;
}
 
/*
 * Use the hash table to map color strings to integer values,
 * like "red" => 0xff0000.
 */
int main() {
	static const char *const keys[] =
	    {"red", "orange", "yellow", "green", "blue", "white", "black"};
	intptr_t value;
	int i;
 
	/* First, create an empty table that can hold 50 entries. */
	if (hcreate(50) == 0)
		fail("hcreate");
 
	/*
	 * Some colors from CSS2,
	 * http://www.w3.org/TR/CSS2/syndata.html#value-def-color
	 */
	store("red",	0xff0000);
	store("orange",	0x123456);  /* Insert wrong value! */
	store("green",	0x008000);
	store("blue",	0x0000ff);
	store("white",	0xffffff);
	store("black",	0x000000);
	store("orange", 0xffa500);  /* Replace with correct value. */
 
	for (i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
		if (fetch(keys[i], &value))
			printf("%s has value %06" PRIxPTR "\n",
			    keys[i], value);
		else
			printf("%s is not in table\n", keys[i]);
	}
 
	/*
	 * DO NOT CALL hdestroy().
	 *
	 * With BSD libc, hdestroy() would call free() with each key in
	 * table. Our keys are static strings, so free() would crash.
	 */
	return 0;
}
