#define _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#define BUF_SIZE 21

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Usage: %s [ port # ] [ # ].\n", argv[0]);
		return(-1);
	}

	int pid = getpid();
	printf("%s process [%d]: starting.\n", argv[0], pid);

	for (int i = 1; i < 3; ++i){
		int str_size = strlen(argv[i]);
		for (int o = 0; o < str_size; ++o){
			if (!(isdigit(argv[i][o]))) {
				fprintf(stderr, "%s process [%d]: %s is not a valid input.\n", argv[0], pid, argv[i]);
				return(2);
			}
		}    
	}

	int nbytes = 0;
	int port_v = atoi(argv[1]);
	int buf = -1;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server;

	if (port_v < 100 || port_v > 65000) {
		fprintf(stderr, "error: invalid port.\n");
		exit(1);
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port_v);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "error: connect failed\n");
		return 2;
	}

	nbytes = recv(sock, &buf, sizeof(int), 0);
	if (nbytes < 0) {
		fprintf(stderr, "error: recv failed.\n");
		return 2;
	}

	printf("%s process [%d]: read %d bytes containing value: %d\n", argv[0], pid, nbytes, buf);
	int one = atoi(argv[2]);

	int result = ((one * buf) & 1);
	if (result) {
		printf("%s process [%d]: %d *IS* even.\n", argv[0], pid, one*buf);
	} else {
		printf("%s process [%d]: %d *IS NOT* even.\n", argv[0], pid, one*buf);
	}
	printf("%s: exiting.\n", argv[0]);
	return(result);
}

