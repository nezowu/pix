CFLAGS= -Wall -O2
LDFLAGS= -lncurses -lmenu
P=rubber
$(P): $(P).c
	gcc $(CFLAGS) $< -o $@ $(LDFLAGS)
