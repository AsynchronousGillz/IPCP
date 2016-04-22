#define _DEFAULT_SOURCE

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#define BUF_SIZE 21

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Usage: %s <pipe> [number].\n", argv[0]);
        return(-1);
    }
    
    int pid = getpid();
    printf("%s process [%d]: starting.\n", argv[0], pid);

    int str_size = strlen(argv[2]);
    for (int o = 0; o < str_size; ++o){
        if (!(isdigit(argv[2][o]))) {
            fprintf(stderr, "%s process [%d]: %s is not a valid input.\n", argv[0], pid, argv[2]);
            return(2);
        }
    }    

    int pipefd = atoi(argv[1]), nbytes;
    int buf = -1;
    while (buf == -1) {
        nbytes = read(pipefd, &buf, sizeof(int));
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

