.PHONY : clean, install, uninstall
CFLAGS= -Wall -Wextra -O0 -g
LDFLAGS= -lncursesw
P=pixy
$(P): pixi.o wch.o hash.o ldir.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

wch.o: wch.c
	gcc $(CFLAGS) -c $<

hash.o: hash.c
	gcc $(CFLAGS) -c $<

listdir.o: ldir.c dir.h
	gcc $(CFLAGS) -c $<

pixie.o: pixi.c my.h dir.h
	gcc $(CFLAGS) -c $<
	
clean :
	rm -f $(P) *~ .*~ .*.swp *.o

install : $(P)
	cp $(P) /usr/bin

uninstall :
	rm /usr/bin/$(P)
