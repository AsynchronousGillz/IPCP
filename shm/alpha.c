// Alpha.c

#define _DEFAULT_SOURCE
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
#define CH "./Beta"
// pipefd[read, write]

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [number]\n", argv[0]);
        return(-1);
    }

    char* number = argv[1];
    int shmid[FORK_NUM]; 
    int *share[FORK_NUM];
    key_t key[FORK_NUM];
    int size = sizeof(int);

    pid_t child_id[FORK_NUM];

    srand(atoi(number));

    for (int i = 0; i < FORK_NUM; i++) {

        key[i] = ftok(".",'a'+i);

        if ((shmid[i] = shmget(key[i], size, 0644 | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error: shmget failed.");
            return(1);
        }
        if ((share[i] = shmat(shmid[i], NULL, 0)) == NULL) {
            fprintf(stderr, "%s: shmat error.\n", argv[0]);
            return(1);
        }
        share[i][0] = rand() % 100000;

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

            char shm_buf[BUF_SIZE];
            sprintf(shm_buf, "%d", shmid[i]);
            execl(full_path, CH, shm_buf, number, NULL);    
            free(full_path);
            
        }else{

            printf("%s: forked process with ID %d\n",argv[0], child_id[i]);
            printf("%s: passed shm ID %d to process %d\n",argv[0], shmid[i], child_id[i]);
            
        } //else
        
    } // for

    for (int i = 0; i < FORK_NUM; i++) {
        int status = 0;
        printf("%s: waiting on child process ID %d...\n",argv[0], child_id[i]);
        waitpid(child_id[i], &status, 0);
        printf("%s: result %d read from shared memort: ", argv[0], share[i][0]);
        
        if (share[i][0]) {
            printf("%s returned %d.\n", CH, status);
        } else {
            printf("%s returned %d.\n", CH, status);
        }
        if (shmdt(share[i])) {
            fprintf(stderr, "shmdt error\n");
            return(1);
        }
        if (shmctl(shmid[i], IPC_RMID, NULL) == -1) {
            fprintf(stderr, "shmctl error\n");
            return(1);
        }
    } // for
    
    printf("%s: exiting.\n", argv[0]);
    return(0);
}
