#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static int keepRunning = 1;

void Handler(int interput) {
	printf("%d found!\n", interput);
	keepRunning = 0;
}

int main(int argc, char *argv[]) {
	struct sigaction act;
	act.sa_handler = Handler;
	sigaction(SIGINT, &act, NULL);
	printf("%s Starting.\n", argv[0]);
	while (keepRunning) pause();
	printf("%s Exiting.\n", argv[0]);
}
