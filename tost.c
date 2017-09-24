#include <ncurses.h>

void ncurses_end(void);
int barmenu(const char**, const int, const int, const int, const int, int, int);
void ncurses_start(void);

int main(void)
{
	int selection=7,row=1, col=10, arraylength=10, wide=15, menulen=5;
	const char *testarray[]={"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
	ncurses_start();

	selection=barmenu(testarray,row,col,arraylength,wide,menulen,selection);

	mvprintw(15,0,"Selection= %d",selection);
	getch();
	ncurses_end();
	return 0;
}
void ncurses_end(void);
int barmenu(const char **array,const int row, const int col, const int arraylength, const int wide, int menulen, int selection)
{
	int counter,offset=0,ky=0;
	char formatstring[7];
	curs_set(0);

	if (arraylength < menulen)
		menulen=arraylength;

	if (selection > menulen)
		offset=selection-menulen+1;

	sprintf(formatstring,"%%-%ds",wide); // remove - sign to right-justify the menu items

	while(ky != 27)
	{
		for (counter=0; counter < menulen; counter++)
		{
			if(counter+offset < 4)
				attron(A_BOLD | COLOR_PAIR(5));
			if (counter+offset==selection)
				attron(A_REVERSE);
			mvprintw(row+counter,col,formatstring,array[counter+offset]);
			attroff(A_REVERSE | A_BOLD | COLOR_PAIR(5));
		}

		ky=getch();

		switch(ky)
		{
			case 'k':
				if (selection)
				{
					selection--;
					if (selection < offset+1)
						if (offset > 0)
							offset--;
				}
				break;
			case 'j':
				if (selection < arraylength-1)
				{
					selection++;
					if (selection > offset+menulen-2)
						if (offset < menulen)
							offset++;
				}
				break;
			case 'g':
				selection=0;
				offset=0;
				break;
			case 'G':
				selection=arraylength-1;
				offset=arraylength-menulen;
				break;
			case 10: //enter
				return selection;
				break;
		}
	}
	return -1;
}

void ncurses_end(void) {
    clear();
    refresh();
    endwin();
}

void ncurses_start()
{
        initscr();
        start_color();
        init_pair(1,COLOR_GREEN,0);
        init_pair(2,COLOR_CYAN,0);
        init_pair(3,COLOR_RED,0);
	init_pair(4,COLOR_YELLOW,0);
	init_pair(5,COLOR_BLUE,0);
	curs_set(0);
        noecho();
//        keypad(stdscr, TRUE);
}
