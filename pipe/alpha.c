// Alpha.c

#define _DEFAULT_SOURCE
#include <ctype.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#define BUF_SIZE 21
#define FORK_NUM 2
#define CH "Beta"
// pipefd[read, write]

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [number].\n", argv[0]);
        return(-1);
    }
    int pid = getpid();

    int str_size = strlen(argv[1]);
    for (int o = 0; o < str_size; ++o){
        if (!(isdigit(argv[1][o]))) {
            fprintf(stderr, "%s process [%d]: %s is not a valid input.\n", argv[0], pid, argv[1]);
            return(2);
        }
    }

    srand(atoi(argv[1]));

    int pipefd[FORK_NUM][2]; 
    int buffer[FORK_NUM];
    int nbytes[FORK_NUM];
    int r_val[FORK_NUM];

    pid_t child_id[FORK_NUM];

    for (int i = 0; i < FORK_NUM; i++) {

        if(pipe(pipefd[i]) == -1) {
            fprintf(stderr, "Error: pipe() failed.\n");
            exit(1);
        }

        r_val[i] = -1;
        nbytes[i] = write(pipefd[i][1], &r_val[i], sizeof(buffer[i]));

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

            char pipe_buf[BUF_SIZE];
            sprintf(pipe_buf, "%d", pipefd[i][0]);
            execl(full_path, CH, pipe_buf, argv[1], NULL);    
            free(full_path);
            
        }else{

            r_val[i] = rand() % 100000;
            nbytes[i] = write(pipefd[i][1], &r_val[i], sizeof(buffer[i]));
            printf("%s: forked process with ID %d\n",argv[0], child_id[i]);
            printf("%s: wrote number %d to pipe (%d bytes)\n",argv[0], r_val[i], nbytes[i]);
            
        } //else
        
    } // for

    for (int i = 0; i < FORK_NUM; i++) {
        int status = 0;
        printf("%s: waiting on child process ID %d...\n",argv[0], child_id[i]);
        waitpid(child_id[i], &status, 0);
        int val = r_val[i] * atoi(argv[1]);
        if (status) {
            printf("%d is not even as %s returned %d.\n", val, CH, status);
        } else {
            printf("%d is even as %s returned %d.\n", val, CH, status);
        }
    } // for
    
    printf("%s: exiting.\n", argv[0]);
    return(0);
}
