#Makefile

# Compiler and loader commands and flags
GCC             = gcc
P_GCC_FLAGS       = -g -std=c99 -Wall -O0 -D_POSIX_C_SOURCE
GCC_FLAGS       = -g -std=c99 -Wall -O0

EXE 			= Ctrl-C POSIX_Ctrl-C

defualt: all

# Target is the executable
all: Ctrl-C POSIX_Ctrl-C

# Compile .c files
Ctrl-C: C.c
	$(GCC) $(GCC_FLAGS) $< -o $@

POSIX_Ctrl-C: C.c
	$(GCC) $(P_GCC_FLAGS) $< -o $@

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE)
