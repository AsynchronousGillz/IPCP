#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void  INThandler(int sig) {
	char  c;
	signal(sig, SIG_IGN);
	printf("WARNING Ctrl-C detected.\nDo you really want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y')  exit(0);
	else signal(SIGINT, INThandler);
	getchar(); // Get new line character
}

int  main(void) {
	signal(SIGINT, INThandler);
	while (1) pause();
	return 0;
}

