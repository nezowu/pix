// консольный файловый пейджер pixie.c wch.c hash.c ldir.c dir.h my.h
#include "my.h"
#define SIZ 256

bool flag; //урать из глобальной после создания 3 колонки

int main() {
	struct dirent **entry, **entry_p;
	CURS = 0;
	OFFSET = 0;
	flag = true; //поставить тип bool - убать в локальную
	char *current;
	char buf[SIZ];
	int key = 0;
	setlocale(LC_ALL, "");
	signal(SIGWINCH, sig_handler);
	signal(SIGINT, sig_handler);
	start_ncurses();
	entry = pwd(&RAW, getcwd(buf, SIZ), flag);
	entry_p = pwd(&PREV, dirname(buf), flag);
	if(RAW.ar_len < LINES-2)
		MENULEN = RAW.ar_len;
	else
		MENULEN = LINES-2;
	cadr();
	cadr_p();
//	initHash();
	while((key = getch()) != ERR) {
		switch(key) {
			case 'j':
				if(RAW.ar_len > 1) {
					if (CURS < RAW.ar_len - 1) {
						CURS++;
						if (CURS > OFFSET + MENULEN - 1)
							OFFSET++;
						cadr();
					}
				}
				break;
			case 'k':
				if(RAW.ar_len > 1) {
					if (CURS) {
						CURS--;
						if (CURS < OFFSET)
							OFFSET--;
						cadr();
					}
				}
				break;
			case 'G':
				if(RAW.ar_len > 1) {
					CURS = RAW.ar_len-1;
					OFFSET = 0;
					if(CURS > OFFSET + MENULEN-1)
						OFFSET = CURS - MENULEN+1;
					cadr();
				}
				break;
			case 'g':
				if(RAW.ar_len > 1) {
					CURS = 0;
					OFFSET = 0;
					cadr();
				}
				break;
			case 'h':
				if(!strcasecmp(getcwd(buf, SIZ), "/")) {
					searchHash(buf, RAW.ar[CURS]->d_name); //запишем или перезапишем базу
					break;
				}
				if(RAW.ar_len > 0)
					searchHash(buf, RAW.ar[CURS]->d_name); //запишем или перезапишем базу
				CURS = 0;
				OFFSET = 0;
				MENULEN = 0;
				current = strdup(basename(buf));
				chdir(dirname(buf));
				reset(entry);
				entry = pwd(&RAW, buf, flag);
				reset_p(entry_p);
				entry_p = pwd(&PREV, dirname(buf), flag);

				for(int i = 0; i < RAW.ar_len; i++) {
					if(!strcasecmp(current, RAW.ar[i]->d_name)) {
						CURS = i;
						break;
					}
				}

				if(RAW.ar_len < LINES-2)
					MENULEN = RAW.ar_len;
				else
					MENULEN = LINES-2;
				if(CURS > OFFSET + MENULEN-1)
					OFFSET = CURS - MENULEN+1;

				cadr();
				cadr_p();
				free(current);
				break;
			case 'l':
				if(RAW.ar_len > 0) {
					if(ACCESS)
						break;
					getcwd(buf, SIZ);
					reset_p(entry_p);
					entry_p = pwd(&PREV, buf, flag);
					//				searchHash(buf, RAW.ar[CURS]->d_name); //запишем или перезапишем в базу
					if(buf[1] != '\0')
						strcat(buf, "/");
					strcat(buf, RAW.ar[CURS]->d_name);
					chdir(buf);

					CURS = 0;
					OFFSET = 0;
					MENULEN = 0;
					reset(entry);
					entry = pwd(&RAW, buf, flag);
					if((current = searchHash(buf, "")) != NULL) {
						for(int i = 0; i < RAW.ar_len; i++) {
							if(!strcasecmp(current, RAW.ar[i]->d_name)) {
								CURS = i;
								break;
							}
						}
					}

					if(RAW.ar_len < LINES-2)
						MENULEN = RAW.ar_len;
					else
						MENULEN = LINES-2;
					if(CURS > OFFSET + MENULEN-1)
						OFFSET = CURS - MENULEN+1;
					cadr();
					cadr_p();
				}
				break;
			case 'a':
				flag = (flag)? false: true;	
				current = strdup(RAW.ar[CURS]->d_name);
				OFFSET = 0;
				CURS = 0;
				reset(entry);
				entry = pwd(&RAW, getcwd(buf, SIZ), flag);
				for(int i = 0; i < RAW.ar_len; i++) {
					if(!strcasecmp(current, RAW.ar[i]->d_name)) {
						CURS = i;
						break;
					}
				}
				if(RAW.ar_len < LINES-2)
					MENULEN = RAW.ar_len;
				else
					MENULEN = LINES-2;
				if(CURS > OFFSET + MENULEN-1)
					OFFSET = CURS - MENULEN+1;

				reset_p(entry_p);
				entry_p = pwd(&PREV, dirname(buf), flag);

				cadr();
				cadr_p();
				free(current);
				break;
			case 'q':
				delwin(Prev);
				delwin(Raw);
				delwin(Next);
				endwin();
				exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}
	return 0;
}

void cadr_p() {
	char buf[SIZ];
//	struct stat sb;
	size_t size;
	int i, j, offset = 0;
	char format_side[7];
	int C4 = COLS / 4 - 2;
	getcwd(buf, SIZ);
	wclear(Prev);
	if(buf[1] == '\0') { //проверка директории на корень "/"
		sprintf(format_side, "%%-%ds", C4);
		wattron(Prev, A_REVERSE | A_BOLD | COLOR_PAIR(5));
		mvwprintw(Prev, 0, 1, format_side, buf);
		wattroff(Prev, A_REVERSE | A_BOLD | COLOR_PAIR(5));
	}
	else {
		for(i = 0; i < PREV.ar_len; i++) {
			if(!strcasecmp(PREV.ar[i]->d_name, basename(buf))) {
				if(i > LINES - 3)
				offset = i - (LINES - 3);
				break;
			}
		} // усли не найдено совпадений присвоить i первый индекс, фиг там!
		for(j = offset; j < PREV.ar_len; j++) {
			int one = 0;
			size = bytesInPos(PREV.ar[j]->d_name, C4, &one);
			memcpy(buf, PREV.ar[j]->d_name, size);
			buf[size] = '\0';
			sprintf(format_side, "%%-%ds", C4 + one);
			if(PREV.ar[j]->d_type == DT_DIR)
				wattron(Prev, A_BOLD | COLOR_PAIR(5));
//			if(PREV.ar[j]->d_type == DT_LNK)
//				wattron(Prev, COLOR_PAIR(2));
//			if(PREV.ar[j]->d_type == DT_REG) {
//				lstat(RAW.ar[j]->d_name, &sb);
//				if(sb.st_mode & S_IXOTH)
//					wattron(Prev, COLOR_PAIR(1));
//			}
			if(j == i) {
				wattron(Prev, A_REVERSE);
			}
			mvwprintw(Prev, j-offset, 1, format_side, buf);
//			wattroff(Prev, A_REVERSE | A_BOLD | COLOR_PAIR(5) | COLOR_PAIR(2));
			wattroff(Prev, A_REVERSE | A_BOLD | COLOR_PAIR(5));
			if(j-offset == LINES - 3)
				break;
		}
	}
	wrefresh(Prev);
}

void cadr() {
	ACCESS = 1;
//	struct stat sb;

	int add_format = 0;
	size_t temp;
	char buf[SIZ];
	int i, j;
	int C2 = COLS / 2 - 2;
	int C4 = COLS / 4 - 2;
	char format_raw[7], format_side[7];
	sprintf(format_side, "%%-%ds", C4); //-2
//	sprintf(format_raw, "%%-%ds", COLS/2-2);

//	wclear(Raw);
//	wclear(Next);
//	clear();

//	box(Prev, 0, 0);
//	box(Raw, 0, 0);
//	box(Next, 0, 0);

	DIR *dir;
	struct dirent *entry_prev;
//	char *str_line;

	wclear(Raw);
	wclear(Next);
	if(RAW.ar_len > 0) { //главная колонка
		for(i = OFFSET, j = 0; j < MENULEN; i++, j++) {
			if(RAW.ar[i]->d_type == DT_DIR)
				wattron(Raw, A_BOLD | COLOR_PAIR(5));
			if(RAW.ar[i]->d_type == DT_LNK)
				wattron(Raw, COLOR_PAIR(2));
			if(RAW.ar[i]->d_type == DT_REG) {
//				lstat(RAW.ar[i+OFFSET]->d_name, &sb);
//				if(sb.st_mode & S_IXOTH)
				if(access(RAW.ar[i]->d_name, X_OK) == F_OK)
					wattron(Raw, COLOR_PAIR(1));
			}
			if(i == CURS)
				wattron(Raw, A_REVERSE);
			temp = bytesInPos(RAW.ar[i]->d_name, C2, &add_format);
			memcpy(buf, RAW.ar[i]->d_name, temp);
			buf[temp] = '\0';

			sprintf(format_raw, "%%-%ds", C2 + add_format);

			mvwprintw(Raw, j, 1, format_raw, buf);
			wattroff(Raw, A_REVERSE | A_BOLD | COLOR_PAIR(5) | COLOR_PAIR(2));
		}

		if(RAW.ar[CURS]->d_type == DT_DIR ) { //выводим на экран третий столбец
			dir = opendir(RAW.ar[CURS]->d_name); //access();
			if(dir) {
				ACCESS = 0;
				for(i = 0; (entry_prev = readdir(dir)) != NULL; i++) {
					if(!strcasecmp(entry_prev->d_name, "..") || !strcasecmp(entry_prev->d_name, ".")) {
						i--;
						continue;
					}
					if(flag && entry_prev->d_name[0] == '.') { //прячем скрытые файлы флаг flag не глобальный
						i--;
						continue;
					}
					add_format = 0;
					temp = bytesInPos(entry_prev->d_name, C4, &add_format);
					memcpy(buf, entry_prev->d_name, temp);
					buf[temp] = '\0';
					sprintf(format_side, "%%-%ds", C4 + add_format);
					if(i == 0) {
						wattron(Next, A_REVERSE);
						mvwprintw(Next, i, 1, format_side, buf);
						wattroff(Next, A_REVERSE);
					}
					else
						mvwprintw(Next, i, 1, format_side, buf);
				}
				closedir(dir);
				if(!i) {
					memcpy(buf, "Empty", 6);
					if(C4 < 5 && C4 > 0)
						buf[C4] = '\0';
					else if(C4 <= 0)
						buf[1] = '\0';
					sprintf(format_side, "%%-%ds", C4);
					wattron(Next, COLOR_PAIR(3));
					mvwprintw(Next, 0, 1, format_side, buf);
					wattroff(Next, COLOR_PAIR(3));
				}
			}
			else {
				ACCESS = 1;
				memcpy(buf, "Not accessible", 15);
				if(C4 < 15 && C4 > 0)
					buf[C4] = '\0';
				else if(C4 <= 0)
					buf[1] = '\0';
				sprintf(format_side, "%%-%ds", C4);
				wattron(Next, COLOR_PAIR(3));
				mvwprintw(Next, 0, 1, format_side, buf);
				wattroff(Next, COLOR_PAIR(3));
			}
		}
	}
	else {
		memcpy(buf, "Empty", 6);
		if(C2 < 5 && C2 > 0)
			buf[C2] = '\0';
		else if(C2 <= 0)
			buf[1] = '\0';
		sprintf(format_raw, "%%-%ds", C2); //+add_format
		wattron(Raw, COLOR_PAIR(3));
		mvwprintw(Raw, 0, 1, format_raw, buf);
		wattroff(Raw, COLOR_PAIR(3));
	}
	wrefresh(Next);
	wrefresh(Raw);

//	refresh();
}

void start_ncurses(void) {
	initscr();
	endwin(); //что это? не едут лыжы!
	cbreak();
	start_color();
	init_pair(1,COLOR_GREEN,0);
	init_pair(2,COLOR_CYAN,0);
	init_pair(3,COLOR_RED,0);
	init_pair(4,COLOR_YELLOW,0);
	init_pair(5,COLOR_BLUE,0);
	noecho();
	curs_set(FALSE);
	int Y = LINES-2;
	int X = COLS/4;
	Prev = newwin(Y, X, 1, 0);
	Raw = newwin(Y, COLS/2, 1, X);
	Next = newwin(Y, X, 1, COLS/2+X);
	refresh();
}

void sig_handler(int signo) {
	if(signo == SIGWINCH) {
		endwin();
//		clear();
		refresh();
//		wresize(stdscr, COLS, LINES); //
		int X = COLS/4;
		int Y = LINES-2;
		wresize(Prev, Y, X);
		wresize(Raw, Y, COLS/2);
		wresize(Next, Y, X);
//		mvwin(Prev, 1, 0); //точка привязки не изменилась
		mvwin(Raw, 1, X);
		mvwin(Next, 1, COLS/2+X);
		OFFSET = 0;
		if(RAW.ar_len < LINES-2)
			MENULEN = RAW.ar_len;
		else
			MENULEN = LINES-2;
		if(CURS > OFFSET + MENULEN-1)
			OFFSET = CURS - MENULEN+1;
		refresh();
		cadr();
		cadr_p();
	}
	else if(signo == SIGINT) {
//END_PROG:
		delwin(Prev);
		delwin(Raw);
		delwin(Next);
		endwin();
		fprintf(stderr, "%s\n", "Good bye! See you!");
			exit(1);
	}
}

void reset(struct dirent ** entry) {
	for(int i = 0; i < RAW.len; i++) {
		free(entry[i]);
	}
	free(entry);
	free(RAW.ar);
}
void reset_p(struct dirent ** entry_p) {
	for(int i = 0; i < PREV.len; i++) {
		free(entry_p[i]);
	}
	free(entry_p);
	free(PREV.ar);
}
