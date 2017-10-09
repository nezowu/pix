.PHONY : clean, install, uninstall
CFLAGS= -Wall -Wextra -O2
LDFLAGS= -lncursesw
P=rain
$(P): $(P).o wch.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

wch.o: wch.c
	gcc -c $<

rain.o: rain.c
	gcc -c $<
	
clean :
	rm -f $(P) *~ .*~ .*.swp *.o

install : $(P)
	cp $(P) /usr/local/bin
