// a.c
//
// 

#define _DEFAULT_SOURCE
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

int main(int agrc, char* argv[]) {
    int pipefd[2];
    int child_id = -1;

    if (pipe(pipefd) == -1){
        fprintf(stderr, "error, failed to pipe\n");
        return(1);
    }
    if ((child_id = fork()) == -1) {
        fprintf(stderr, "error, failed to pipe\n");
        return(1);
    }

    if (child_id == 0) {
        // child

    } else {
        // parent
        
    }

    return(0);
}
