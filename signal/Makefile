#Makefile

# Compiler and loader commands and flags
GCC             = gcc
GCC_FLAGS       = -g -std=c99 -Wall -O0

EXE 			= Ctrl-C

defualt: all

# Target is the executable
all: Ctrl-C

# Compile .c files
Ctrl-C: C.c
	$(GCC) $(GCC_FLAGS) $< -o $@

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE)
