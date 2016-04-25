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
        printf("Usage: %s <shm> <number>\n", argv[0]);
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

    int shm = atoi(argv[1]);
    int *share;
    
    if ((share = shmat(shm, NULL, 0)) == NULL) {
        fprintf(stderr, "%s: shmat error.\n", argv[0]);
        return(1);
    }
    printf("%s process [%d]: result %d read from shared memort.\n", argv[0], pid, *share);


    int one = atoi(argv[2]);
    
    int result = ((one * (*share)) & 1)?1:0;
    if (result) {
        printf("%s process [%d]: %d is odd.\n", argv[0], pid, one*(*share));
    } else {
        printf("%s process [%d]: %d is even.\n", argv[0], pid, one*(*share));
    }
    *share = result;
    printf("%s process [%d]: wrote result (%d) to shared memory.\n", argv[0], pid, result);

    shmdt(share);
    return(0);
}

