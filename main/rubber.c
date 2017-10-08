// консольный файловый пейджер rubber.c
#include "my.h"

time_t START;
int CURS = 0;
int OFFSET = 0;
int MENULEN;
int ACCESS;
WINDOW *Prev, *Raw, *Next;

typedef struct col {
	struct dirent **ar;
	int ar_len;
} Col;

Col PREV, RAW, NEXT;

void cadr();
static void sig_handler(int);
void start_ncurses(void);
//int charwidth(char *, int *);
int bytesInPos(char *, int);
void pwd(struct col *);

int main() {
	char buf[128];
	int key = 0;
	START = time(NULL);
	setlocale(LC_ALL, "");
	signal(SIGWINCH, sig_handler);
	signal(SIGINT, sig_handler);
	start_ncurses();
	pwd(&RAW);
	if(RAW.ar_len < LINES-2)
		MENULEN = RAW.ar_len;
	else
		MENULEN = LINES-2;

	cadr();
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
				if(!strcasecmp(getcwd(buf, 128), "/"))
					break;
				CURS = 0;
				OFFSET = 0;
				MENULEN = 0;

				for(int i = 0; i < RAW.ar_len; i++) {
					free(RAW.ar[i]);
				}
				free(RAW.ar);

				chdir(dirname(getcwd(buf, 128)));

				pwd(&RAW);
				if(RAW.ar_len < LINES-2)
					MENULEN = RAW.ar_len;
				else
					MENULEN = LINES-2;
				cadr();
				break;
			case 'l':
				if(ACCESS)
					break;
				getcwd(buf, 128);
				strcat(buf, "/");
				strcat(buf, RAW.ar[CURS]->d_name);
				chdir(buf);
				memset(buf, 0, 128);

				CURS = 0;
				OFFSET = 0;
				MENULEN = 0;

				for(int i = 0; i < RAW.ar_len; i++) {
					free(RAW.ar[i]);
				}
				free(RAW.ar);

				pwd(&RAW);
				if(RAW.ar_len < LINES-2)
					MENULEN = RAW.ar_len;
				else
					MENULEN = LINES-2;
				cadr();
				break;
			default:
				break;
		}
	}
	return 0;
}

void cadr() {
	ACCESS = 1;
	struct stat sb;
	int i;
	int C4 = COLS / 4 - 2;
	char format_side[7], format_raw[7];
	sprintf(format_side, "%%-%ds", C4-2);
	sprintf(format_raw, "%%-%ds", COLS/2-2);

	wclear(Prev);
	wclear(Raw);
	wclear(Next);
	
	box(Prev, 0, 0);
	box(Raw, 0, 0);
	box(Next, 0, 0);

	struct dirent *entry_prev;
	DIR *dir;
	char buf[128] = {0};
	char currentdir[128] = {0};
	getcwd(currentdir, 128);
	getcwd(buf, 128);
	char *prev_dir;
	dirname(buf);
	char str_line[128] = {0};
	if(!strcasecmp(currentdir, "/")) {
		memcpy(str_line, "/", 1);
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
			memset(str_line, 0, 128);
			memcpy(str_line, entry_prev->d_name, bytesInPos(entry_prev->d_name, C4));
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
      		mvwprintw(Raw, i, 1, format_raw, RAW.ar[i+OFFSET]->d_name);
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
				memset(str_line, 0, 128);
				memcpy(str_line, entry_prev->d_name, bytesInPos(entry_prev->d_name, C4));
				mvwprintw(Next, i, 1, format_side, str_line);
			}
			closedir(dir);
			if(!i) {
				memset(str_line, 0, 128);
				memcpy(str_line, "Empty", bytesInPos("Empty", C4));
				wattron(Next, COLOR_PAIR(3));
				mvwprintw(Next, 0, 1, format_side, str_line);
				wattroff(Next, COLOR_PAIR(3));
			}
		} else {
			ACCESS = 1;
			memset(str_line, 0, 128);
			memcpy(str_line, "Not accessible", bytesInPos("Not accessible", C4));
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
//	keypad(stdscr,TRUE);
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
		delwin(Prev);
		delwin(Raw);
		delwin(Next);
		endwin();
		fprintf(stderr, "%s\n", "Good bye! See you!");
			exit(1);
	}
}

int bytesInPos(char *s, int pos) { //задача нати размер в байтах по позиции и корректно обрезать
	int size;
	int count = 0;
	int flag = 0;
	for (; *s; s++) {
		count++; //считаем байты
		flag++;
		if((*s & 0xC0) != 0x80) {
			flag = 0;
			pos--; //считаем позиции смволов одно и двух байтных
		}
		if(pos < 0) {
			if(flag == 1)
				count--;
			count--;
			break;
		}
		if(flag == 2) { //если мы досчитали до 3 байта, то пора проверить является ли символ широким?
			flag = 0;
			pos -= charwidth(s, &size); //если является то изменим позицию еще на единицу
			if(pos < 0) {
				count -= size;
				break;
			}
		}
	}
	return count;
}

void pwd(struct col *raw) {
	time_t t_raw = START;
	raw->ar_len = 0;
	struct dirent *entry_raw;
	struct stat status;
	int i, count_dir = 0, count_file = 0;
	char buf[128] = {0};
	getcwd(buf, 128);
	DIR *raw_col = opendir(buf);
	size_t len = sizeof(struct dirent);
	for(i = 0; (entry_raw = readdir(raw_col)) != NULL; i++) {
		lstat(entry_raw->d_name, &status); //находим индекс файла с последним временем доступа или 0 со старта
		if(status.st_atime > t_raw) {
			CURS = i;
			t_raw = status.st_atime;
		}
		if(entry_raw->d_type == DT_DIR)
			count_dir++;
		else
			count_file++;
	}
	raw->ar_len = count_dir + count_file;
	count_file = 0;
	rewinddir(raw_col);
	raw->ar = (struct dirent **)calloc(raw->ar_len, sizeof(struct dirent *));
	for(i = 0; i < raw->ar_len; i++)
		raw->ar[i] = (struct dirent *)calloc(1, len);
	for(i = 0; (entry_raw = readdir(raw_col)) != NULL; i++) {
		if(entry_raw->d_type == 4) {
			memcpy((void *)raw->ar[count_file], (void *)entry_raw, len);
			count_file++;
		} else {
			memcpy((void *)raw->ar[count_dir], (void *)entry_raw, len);
			count_dir++;
		}
	}
	closedir(raw_col);
	return;
}
//void pwd_prev(void) {
//	struct dirent *entry_prev;
//	int i;
//	char buf[128] = {0};
//	getcwd(buf, 128);
//	DIR *prev_col = opendir(dirname(buf));
//	for(i = 0; readdir(prev_col) != NULL; i++) {
//		PREV_LEN++;
//	}
//	rewinddir(prev_col);

//int barmenu(const char **array,const int row, const int col, const int arrlen, const int wide, int menulen, int selection)
//{
//	int counter,offset=0,ky=0;
//	char formatstring[7];
//	curs_set(0);
//
//	if (arrlen < menulen)
//		menulen=arrlen;
//
//	if (selection > menulen)
//		offset=selection-menulen+1;
//
//	sprintf(formatstring,"%%-%ds",wide); // remove - sign to right-justify the menu items
//
//	while(1)
//	{
//		for (counter=0; counter < menulen; counter++)
//		{
//			if(counter+offset < 4)
//				attron(A_BOLD | COLOR_PAIR(5));
//			if (counter+offset==selection)
//				attron(A_REVERSE);
//			mvprintw(row+counter,col,formatstring,array[counter+offset]);
//			attroff(A_REVERSE | A_BOLD | COLOR_PAIR(5));
//		}
//
//		ky=getch();
//
//		switch(ky)
//		{
//			case 'k':
//				if (selection)
//				{
//					selection--;
//					if (selection < offset+1)
//						if (offset > 0)
//							offset--;
//				}
//				break;
//			case 'j':
//				if (selection < arrlen-1)
//				{
//					selection++;
//					if (selection > offset+menulen-2)
//						if (offset < menulen)
//							offset++;
//				}
//				break;
//			case 'g':
//				selection=0;
//				offset=0;
//				break;
//			case 'G':
//				selection=arrlen-1;
//				offset=arrlen-menulen;
//				break;
//			case 10: //enter
//				return selection;
//				break;
//		}
//	}
//	return -1;
//}
