#Makefile template for CS370

# Compiler and loader commands and flags
GCC             = gcc
GCC_FLAGS       = -g -std=c99 -Wall -O0

EXE 			= Alpha Beta

defualt: all

# Target is the executable
all: Alpha Beta

# Compile .c files
Alpha: alpha.c
	$(GCC) $(GCC_FLAGS) $< -o $@

Beta: beta.c
	$(GCC) $(GCC_FLAGS) $< -o $@

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE)
