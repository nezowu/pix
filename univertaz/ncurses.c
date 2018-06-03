//просмотртеть вывод функции scanfdir в ncurses с функцией raw()
#define _GNU_SOURCE
#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "");
	struct dirent **entry, **entry_next, **next, **next_next;
	int n, m, i;
	n = scandir("/home/nez/Projects/pix_check", &entry, 0, alphasort);
	scandir("/home/nez/Projects/pix_check", &entry_next, 0, versionsort);
	m = scandir("/home/nez/Projects/pix/univertaz", &next, 0, alphasort);
	scandir("/home/nez/Projects/pix/univertaz", &next_next, 0, versionsort);

        initscr();                      /* Start curses mode            */
//        raw();                          /* Line buffering disabled      */
	curs_set(FALSE); /*убираем курсор*/
	cbreak();
	noecho();
	clear();
	WINDOW *win, *nex;
	win = newwin(LINES, COLS/2, 0, 0);
	nex = newwin(LINES, COLS/2, 0, COLS/2);
	mvwin(nex, 0, COLS/2);
	for(i = 0; i < n; i++) {
		if(entry[i]->d_type == DT_DIR)
			wattron(win, A_BOLD);
		wprintw(win, "%s\n", entry[i]->d_name);
		wattroff(win, A_BOLD);
	}
	for(i = 0; i < n; i++) {
		if(entry_next[i]->d_type == DT_DIR)
			wattron(nex, A_BOLD);
		wprintw(nex, "%s\n", entry_next[i]->d_name);
		wattroff(nex, A_BOLD);
	}
        refresh();                      /* Print it on to the real screen */
	wrefresh(win);
	wrefresh(nex);
        getch();                        /* Wait for user input */
	wclear(win);
	for(i = 0; i < m; i++) {
		if(next[i]->d_type == DT_DIR)
			wattron(win, A_BOLD);
		wprintw(win, "%s\n", next[i]->d_name);
		wattroff(win, A_BOLD);
	}
	wrefresh(win);
	wclear(nex);
	for(i = 0; i < m; i++) {
		if(next_next[i]->d_type == DT_DIR)
			wattron(nex, A_BOLD);
		wprintw(nex, "%s\n", next_next[i]->d_name);
		wattroff(nex, A_BOLD);
	}
	wrefresh(nex);
	getch();
        endwin();                       /* End curses mode                */
        return 0;
}
