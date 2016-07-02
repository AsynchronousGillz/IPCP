// Alpha.c

#define _DEFAULT_SOURCE
#include <ctype.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#define BUF_SIZE 21
#define FORK_NUM 2
#define CH "Beta"
// pipefd[read, write]

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s [ port # ] [ # ]\n", argv[0]);
		return(-1);
	}
	int pid = getpid();

	for (int i = 1; i < 3; ++i){
		int str_size = strlen(argv[i]);
		for (int o = 0; o < str_size; ++o){
			if (!(isdigit(argv[i][o]))) {
				fprintf(stderr, "%s process [%d]: %s is not a valid input.\n", argv[0], pid, argv[i]);
				return(2);
			}
		}
	}

	int r_val[FORK_NUM];
	int nbytes[FORK_NUM];
	int port_v[FORK_NUM];
	int socket_desc[FORK_NUM][2];
	struct sockaddr_in struct_socks[FORK_NUM];
	struct sockaddr_storage struct_client[FORK_NUM];

	pid_t child_id[FORK_NUM];
	srand(atoi(argv[1]));

	for (int i = 0; i < FORK_NUM; i++) {
		port_v[i] = atoi(argv[1])+i;
		if (port_v[i] < 100 || port_v[i] > 65000) {
			fprintf(stderr, "error: invalid port.\n");
			exit(1);
		}

		socket_desc[i][0] = socket(AF_INET, SOCK_STREAM, 0);

		if (socket_desc[i][0] == -1) {
			fprintf(stderr, "Error: pipe() failed.\n");
			exit(1);
		}

		struct_socks[i].sin_family = AF_INET;
		struct_socks[i].sin_addr.s_addr = INADDR_ANY;
		struct_socks[i].sin_port = htons(port_v[i]);

		if (bind(socket_desc[i][0], (struct sockaddr *) &struct_socks[i], sizeof(struct_socks)) < 0) {
			close(socket_desc[i][0]);
			fprintf(stderr, "Error: bind() failed.\n");
			exit(1);
		}

		listen(socket_desc[i][0], 1);

		if ((child_id[i] = fork()) == -1) {
			fprintf(stderr, "Error: fork() failed.\n");
			return(1);
		}

		if (child_id[i] == 0) {
			char buf[PATH_MAX + 1];
			char* full_path = realpath(CH, buf);
			if (full_path == NULL) {
				fprintf(stderr, "Error: %s can not be found.\n", CH);
				exit(1);
			}
			char sock_buf[BUF_SIZE];
			sprintf(sock_buf, "%d", port_v[i]);
			execl(full_path, CH, sock_buf, argv[2], NULL);    
			free(full_path);
		}else{
			printf("%s: forked process with ID %d\n",argv[0], child_id[i]);
		} //else
	} // for

	for (int i = 0; i < FORK_NUM; i++) {
		r_val[i] = rand() % 100000;
		socket_desc[i][1] = accept(socket_desc[i][0], (struct sockaddr *) &struct_client[i], (socklen_t*)sizeof(int));
		if (socket_desc[i][1] < 0) {
			fprintf(stderr, "Error: accept() failed.\n");
			return 1;
		}
		nbytes[i] = send(socket_desc[i][1], &r_val[i], sizeof(int), 0);
		if (nbytes[i] < 0) {
			fprintf(stderr, "error: send failed.\n");
			return 1;
		}
		printf("%s: wrote number %d to socket (%d bytes)\n",argv[0], r_val[i], nbytes[i]);

		int status = 0;
		printf("%s: waiting on child process ID %d...\n",argv[0], child_id[i]);
		waitpid(child_id[i], &status, 0);
		int val = r_val[i] * atoi(argv[2]);
		if (status) {
			printf("%d is not even as %s returned %d.\n", val, CH, status);
		} else {
			printf("%d is even as %s returned %d.\n", val, CH, status);
		}
	} // for

	printf("%s: exiting.\n", argv[0]);
	return(0);
}
