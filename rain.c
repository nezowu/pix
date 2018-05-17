// консольный файловый пейджер rain.c wch.c hash.c my.h
#include "my.h"
#define SIZ 256

int main() {
	CURS = 0;
	OFFSET = 0;
	HIDDEN = 1;
	char *tmp;
	char temp[SIZ] = {0};
	char buf[SIZ];
	int key = 0;
	START = time(NULL);
	setlocale(LC_ALL, "");
	signal(SIGWINCH, sig_handler);
	signal(SIGINT, sig_handler);
	start_ncurses();
	pwd(&RAW, getcwd(buf, SIZ));
	if(RAW.ar_len < LINES-2)
		MENULEN = RAW.ar_len;
	else
		MENULEN = LINES-2;
	cadr();
	initHash();
	while(key = getch()) {
		switch(key) {
			case 'j':
				if (CURS < RAW.ar_len - 1) {
					CURS++;
					if (CURS > OFFSET + MENULEN - 1)
							OFFSET++;
					cadr();
				}
				break;
			case 'k':
				if (CURS) {
					CURS--;
					if (CURS < OFFSET)
							OFFSET--;
					cadr();
				}
				break;
			case 'G':
				CURS = RAW.ar_len-1;
				OFFSET = 0;
				if(CURS > OFFSET + MENULEN-1)
					OFFSET = CURS - MENULEN+1;
				cadr();
				break;
			case 'g':
				CURS = 0;
				OFFSET = 0;
				cadr();
				break;
			case 'h':
				if(!strcasecmp(getcwd(buf, SIZ), "/"))
					break;
				if(RAW.ar_len > 0)
//					atime(RAW.ar[CURS]->d_name);
					searchHash(buf, RAW.ar[CURS]->d_name); //запишем или перезапишем базу
				CURS = 0;
				OFFSET = 0;
				MENULEN = 0;

				for(int i = 0; i < RAW.len; i++) {
					free(RAW.ar[i]);
				}
				free(RAW.ar);
				memcpy(temp, basename(buf), SIZ);
				dirname(buf);
				chdir(buf);

				pwd(&RAW, buf);

				for(int i = 0; i < RAW.ar_len; i++) {
					if(!strcasecmp(temp, RAW.ar[i]->d_name)) {
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
				break;
			case 'l':
				if(ACCESS)
					break;
				getcwd(buf, SIZ);
//				searchHash(buf, RAW.ar[CURS]->d_name); //запишем или перезапишем в базу
				if(buf[1] != '\0')
					strcat(buf, "/");
				strcat(buf, RAW.ar[CURS]->d_name);
				chdir(buf);

				CURS = 0;
				OFFSET = 0;
				MENULEN = 0;

				for(int i = 0; i < RAW.len; i++) {
					free(RAW.ar[i]);
				}
				free(RAW.ar);

				pwd(&RAW, buf);
				if((tmp = searchHash(buf, "")) != NULL) {
					for(int i = 0; i < RAW.ar_len; i++) {
						if(!strcasecmp(tmp, RAW.ar[i]->d_name)) {
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
				break;
			case 'a':
				tmp = RAW.ar[CURS]->d_name;
				OFFSET = 0;
				CURS = 0;
				HIDDEN = (HIDDEN)? 0: 1;	
				pwd(&RAW, getcwd(buf, SIZ));
				for(int i = 0; i < RAW.ar_len; i++) {
					if(!strcasecmp(tmp, RAW.ar[i]->d_name)) {
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
//		memset(buf, 0, SIZ);
	}
	return 0;
}

void cadr() {
	ACCESS = 1;
	struct stat sb;

	int temp, add_format;

	int i;
	int C2 = COLS / 2 - 2;
	int C4 = COLS / 4 - 2;
	char format_side[7], format_raw[7]; //1
	memset(format_side, 0, 7); //2
	sprintf(format_side, "%%-%ds", C4-2); //3
//	sprintf(format_raw, "%%-%ds", COLS/2-2);

	wclear(Prev);
	wclear(Raw);
	wclear(Next);
//	clear();
	
//	box(Prev, 0, 0);
//	box(Raw, 0, 0);
//	box(Next, 0, 0);

	struct dirent *entry_prev;
	DIR *dir;
	char buf[SIZ] = {0};
	char currentdir[SIZ] = {0};
	getcwd(currentdir, SIZ);
	getcwd(buf, SIZ);
	char *prev_dir;
	dirname(buf);
	char str_line[SIZ] = {0};
	if(!strcasecmp(currentdir, "/")) {
		memcpy(str_line, "/", 2);
		mvwprintw(Prev, 0, 1, format_side, str_line);
	} else {
		dir = opendir(buf);
		for(i = 0;(entry_prev = readdir(dir)) != NULL; i++) {
			if(!strcasecmp(entry_prev->d_name, "..") || !strcasecmp(entry_prev->d_name, ".")) {
				i--;
				continue;
			}
			else if(strchr(".", entry_prev->d_name[0])) { //прячем скрытые файлы
				i--;
				continue;
			}
			memset(str_line, 0, SIZ);
			int one;
			memcpy(str_line, entry_prev->d_name, bytesInPos(entry_prev->d_name, C4, &one));
			mvwprintw(Prev, i, 1, format_side, str_line);
		}
		closedir(dir);
	}

	for(i = 0; i < MENULEN; i++) {
		if(RAW.ar[i+OFFSET]->d_type == DT_DIR)
			wattron(Raw, A_BOLD | COLOR_PAIR(5));
		if(RAW.ar[i+OFFSET]->d_type == DT_LNK)
			wattron(Raw, COLOR_PAIR(2));
		if(RAW.ar[i+OFFSET]->d_type == DT_REG) {
			lstat(RAW.ar[i+OFFSET]->d_name, &sb);
			if(sb.st_mode & S_IXOTH)
				wattron(Raw, COLOR_PAIR(1));
		}
		if(i+OFFSET == CURS)
			wattron(Raw, A_REVERSE);
		temp = bytesInPos(RAW.ar[i+OFFSET]->d_name, C2, &add_format);
		memset(str_line, 0, SIZ);
		memcpy(str_line, RAW.ar[i+OFFSET]->d_name, temp);

		memset(format_raw, 0, 7); //4
		sprintf(format_raw, "%%-%ds", COLS/2-2 + add_format); //5

     		mvwprintw(Raw, i, 1, format_raw, str_line);
		wattroff(Raw, A_REVERSE | A_BOLD | COLOR_PAIR(5) | COLOR_PAIR(2));
	}
	/*выводим на экран третий столбец*/
	if(RAW.ar[CURS]->d_type == DT_DIR ) {
		dir = opendir(RAW.ar[CURS]->d_name);
		if(dir) {
			ACCESS = 0;
			for(i = 0; (entry_prev = readdir(dir)) != NULL; i++) {
				if(!strcasecmp(entry_prev->d_name, "..") || !strcasecmp(entry_prev->d_name, ".")) {
					i--;
					continue;
				}
				else if(strchr(".", entry_prev->d_name[0])) { //прячем скрытые файлы
					i--;
					continue;
				}
				memset(str_line, 0, SIZ);
				int two;
				memcpy(str_line, entry_prev->d_name, bytesInPos(entry_prev->d_name, C4, &two));
				mvwprintw(Next, i, 1, format_side, str_line);
			}
			closedir(dir);
			if(!i) {
				memset(str_line, 0, SIZ);
				int three;
				memcpy(str_line, "Empty", bytesInPos("Empty", C4, &three));
				wattron(Next, COLOR_PAIR(3));
				mvwprintw(Next, 0, 1, format_side, str_line);
				wattroff(Next, COLOR_PAIR(3));
			}
		} else {
			ACCESS = 1;
			memset(str_line, 0, SIZ);
			int four;
			memcpy(str_line, "Not accessible", bytesInPos("Not accessible", C4, &four));
			wattron(Next, COLOR_PAIR(3));
			mvwprintw(Next, 0, 1, format_side, str_line);
			wattroff(Next, COLOR_PAIR(3));
		}
	}

	refresh();
	wrefresh(Prev);
	wrefresh(Raw);
	wrefresh(Next);
}

void start_ncurses(void) {
	initscr();
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
}

static void sig_handler(int signo) {
	if(signo == SIGWINCH) {
		int tmp;
		endwin();
		refresh();
		wclear(Prev);
		wclear(Raw);
		wclear(Next);
//		clear();
//		wresize(stdscr, COLS, LINES); //
		int X = COLS/4;
		int Y = LINES-2;
		wresize(Prev, Y, X);
		wresize(Raw, Y, COLS/2);
		wresize(Next, Y, X);
		mvwin(Raw, 1, X);
		mvwin(Next, 1, COLS/2+X);
		OFFSET = 0;
		if(RAW.ar_len < LINES-2)
			MENULEN = RAW.ar_len;
		else
			MENULEN = LINES-2;
		if(CURS > OFFSET + MENULEN-1)
			OFFSET = CURS - MENULEN+1;
		cadr();
	}
	else if(signo == SIGINT) {
END_PROG:
		delwin(Prev);
		delwin(Raw);
		delwin(Next);
		endwin();
		fprintf(stderr, "%s\n", "Good bye! See you!");
			exit(1);
	}
}

int pwd(struct col *raw, char *path) {
	int i, ar_len;
	int flag = HIDDEN;
	time_t t_raw = START;
	struct dirent **entry;
	struct stat status;
	int count_dir = 0, count_file = 0, count_hid = 0, count_hiddir = 0;
	ar_len = scandir(path, &entry, 0, alphasort);
	raw->ar = (struct dirent **)calloc(ar_len, sizeof(struct dirent *));
	for(i = 0; i < ar_len; i++) {
		raw->ar[i] = (struct dirent *)calloc(1, sizeof(struct dirent));
		if(entry[i]->d_type == DT_DIR) {
			count_dir++;
			if(entry[i]->d_name[0] == '.')
				count_hiddir++;
		}
	}

	if(flag)
		count_dir = count_dir - count_hiddir;
	else
		count_dir -= 2;
	for(i = 2; i < ar_len; i++) {
		if(entry[i]->d_type == DT_DIR) {
			if(flag && entry[i]->d_name[0] == '.')
				continue;
			memcpy((void *)raw->ar[count_file], (void *)entry[i], sizeof(struct dirent));
			count_file++;
		} else {
			if(flag && entry[i]->d_name[0] == '.')
				continue;
			memcpy((void *)raw->ar[count_dir], (void *)entry[i], sizeof(struct dirent));
			count_dir++;
		}
	}
	for(i = 0; i < ar_len; i++)
		free(entry[i]);
	free(entry);
//	for(i = 0; i < count_dir; i++) { //if own == $user else
//		lstat(raw->ar[i]->d_name, &status);
//		if(status.st_atime > t_raw) {
//			ind = i;
//			t_raw = status.st_atime;
//		}
//	}
	raw->ar_len = count_dir;
	raw->len = ar_len;
	//return ind;
	return EXIT_SUCCESS;
}

//void atime(char *path) {
//	struct stat status;
//	struct utimbuf buf;
//	if(lstat(RAW.ar[CURS]->d_name, &status) == -1) { //if own == $user else
//		perror("lstat:403");
//		exit(EXIT_FAILURE);
//	}
//	buf.modtime = status.st_mtime;
//	buf.actime = time(NULL);
//	utime(RAW.ar[CURS]->d_name, &buf);
//}
//void pwd_prev(void) {
//	struct dirent *entry_prev;
//	int i;
//	char buf[SIZ] = {0};
//	getcwd(buf, SIZ);
//	DIR *prev_col = opendir(dirname(buf));
//	for(i = 0; readdir(prev_col) != NULL; i++) {
//		PREV_LEN++;
//	}
//	rewinddir(prev_col);
