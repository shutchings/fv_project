# Written by Steve Hutchings
# This make file compiles each C file into it's own executable.
# The name of the executable is the same as the source file minus the .c extension
# Note: each source file MUST have a entry point, otherwise this makefile will fail

# the compiler to use
CC=gcc

# compiler flags
CFLAGS=-O2 -Wall -Wextra -Werror -pthread -lm

#preproccessor flags
CPPFLAGS=

# non-library linker flags
LDFLAGS=

# libraries to pass to the linker
LDLIBS=

# command used to delete files
RM=rm -f

# the shell make is to use
SHELL=/bin/sh

# set the suffixes this makefile will use
.SUFFIXES:
.SUFFIXES: .c .o

# get a list of all c files
SOURCES=$(wildcard *.c)

# build a list of executable names
EXECUTABLES=$(SOURCES:.c=)

all: $(EXECUTABLES)

.c:
        $(CC) -I. $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

clean:
        $(RM) $(EXECUTABLES)

run1: problem_1
        taskset -c 1 ./problem_1 $(threads) $(seconds)

run2: problem_2
        taskset -c 1 ./problem_2 $(threads) $(seconds)

run3: problem_3
        ./problem_3 $(threads) $(seconds)

run4: problem_4
        ./problem_4 $(threads) $(seconds)

run5: problem_5
        ./problem_5 $(threads) $(seconds)

run6: problem_6
        ./problem_6 $(threads) $(seconds)
                                             