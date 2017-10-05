CFLAGS= -Wall -Wextra -O2
LDFLAGS= -lncursesw
P=rubber
$(P): $(P).o rub.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

rub.o: rub.c
	gcc -c $<

rubber.o: rubber.c
	gcc -c $<
