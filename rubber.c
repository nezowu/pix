// rubber.c
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

WINDOW *Prev, *Raw, *Next;

void cadr();
static void sig_handler(int);
void start_ncurses(void);

int main(int argc, char *argv[]) {
	signal(SIGWINCH, sig_handler);
	signal(SIGINT, sig_handler);
	start_ncurses();
	cadr();
	while(1) {
		sleep(5);
	}
	return 0;
}

void cadr() {
	box(Prev, 0, 0);
	box(Raw, 0, 0);
	box(Next, 0, 0);

	mvwprintw(Prev, 1, 1, "Prev");
	mvwprintw(Raw, 1, 1, "Raw");
	mvwprintw(Next, 1, 1, "Next");
	wrefresh(Prev);
	wrefresh(Raw);
	wrefresh(Next);
}

void start_ncurses(void) {
	initscr();
	noecho();
	curs_set(FALSE);
	int Y = LINES-2;
	int X = COLS/4;
	Prev = newwin(Y, X, 1, 0);
	Raw = newwin(Y, COLS/2, 1, X);
	Next = newwin(Y, X, 1, COLS/2+X);
}

static void sig_handler(int signo) {
	if(signo == SIGWINCH) {
		endwin();
		refresh();
		clear();
		wclear(Prev);
		wclear(Raw);
		wclear(Next);
		int X = COLS/4;
		int Y = LINES-2;
		wresize(Prev, Y, X);
		wresize(Raw, Y, COLS/2);
		wresize(Next, Y, X);
		mvwin(Raw, 1, X);
		mvwin(Next, 1, COLS/2+X);
		cadr();
	}
	else if(signo == SIGINT) {
		delwin(Prev);
		delwin(Raw);
		delwin(Next);
		endwin();
		fprintf(stderr, "%s\n", "Good bye! See you!");
			exit(1);
	}
}
