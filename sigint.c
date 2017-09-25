#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include <stdlib.h>

static void sig_handler(int signo)
{
	if (signo == SIGWINCH)
		printf("received SIGWINCH изменился размер\n");
	else if (signo == SIGINT) {
		printf("ninau nikavo\n");
		exit(0);
	}
}

int main(void)
{
	signal(SIGWINCH, sig_handler);
	signal(SIGINT, sig_handler);
//	if (signal(SIGWINCH, sig_handler) == SIG_ERR)
//		printf("\ncan't catch SIGINT\n");
//	if (signal(SIGINT, sig_handler) == SIG_ERR)
//		printf("\nошибак какато\n");
	while(1) 
		sleep(1);
	return 0;
}
