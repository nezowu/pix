CFLAGS= -Wall -O2
LDLIBS= -lncurses -lmenu
P=tost
$(P): $(P).c
	gcc $(CFLAGS) $< -o $@ $(LDLIBS)
