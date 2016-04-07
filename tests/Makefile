# Written by Steve Hutchings
# This make file compiles each C file into it's own executable.
# The name of the executable is the same as the source file minus the .c extension
# Note: each source file MUST have a entry point, otherwise this makefile will fail

# the compiler to use
CC=gcc

# compiler flags
CFLAGS= -pthread -O2 -fmessage-length=0 -pedantic-errors -std=gnu99 -Werror -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-include-dirs -Wno-unused-parameter -Wshadow -Wuninitialized -Wold-style-definition

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
EXECUTABLES=$(SOURCES:.c=.o)

all: $(EXECUTABLES)

.c:
	$(CC) -I. $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

clean:
	$(RM) $(EXECUTABLES)

