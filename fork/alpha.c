// a.c

#define _DEFAULT_SOURCE
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

void child (char* i) {
    const char* file_name = "Beta";
    char buf[PATH_MAX + 1];
    char* full_path = realpath(file_name, buf);
    if (full_path == NULL) {
        fprintf(stderr, "Error: %s can not be found.\n", file_name);
        exit(1);
    }
    execl(full_path, file_name, i, NULL);
    free(full_path);
}

void parent (int* pid) {
    int ret = 0;

    printf("Alpha: forked process with ID %d\n", *pid);
    printf("Alpha: waiting for process [%d].\n", *pid);
    
    wait(&ret);
    int ret_Val = WEXITSTATUS(ret);
    if (ret_Val == 1){
        printf("Alpha: error input was not valid.\n");
    } else {
        printf("Alpha: child process %d returned %d.\n", *pid, ret_Val);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [number].\n", argv[0]);
        return(-1);
    }
    int num = atoi(argv[1]);

    for (int i = 0; i < num; i += 3) {
        
        pid_t child_id;
        if ((child_id = fork()) == -1) {
            fprintf(stderr, "\e[1;31mfork() failed.\e[0m\n");
            exit(1);
        }

        if (child_id != 0) {
            parent(&child_id);
        }else{
            child(argv[1]);
        }
    }
    printf("Alpha: exiting.\n");
    return(0);
}

