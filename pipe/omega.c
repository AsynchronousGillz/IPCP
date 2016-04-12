// omega.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [number].\n", argv[0]);
        return(-1);
    }
    int num = atoi(argv[1]);
    int ret = 0;
    for (int i = num; i > 0; i--) {
        ret += i;
    }
    printf("Omega: exiting.\n");
    return(ret);
}
