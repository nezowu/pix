.PHONY : clean, install, uninstall
CFLAGS= -Wall -Wextra -O2
LDFLAGS= -lncursesw
P=rain
$(P): $(P).o wch.o hash.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

wch.o: wch.c
	gcc -c $<

hash.o: hash.c
	gcc -c $<

rain.o: rain.c
	gcc -c $<
	
clean :
	rm -f $(P) *~ .*~ .*.swp *.o

install : $(P)
	cp $(P) /usr/bin

uninstall :
	rm /usr/bin/$(P)
