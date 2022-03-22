.PHONY:all

TARGETS=cmd_format

all: $(TARGETS)

clean:
	@rm -f *.o *~ $(TARGETS) *.d

CC=gcc
CFLAGS=-Wall -std=c99
sources=$(wildcard *.c)

# compile
%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

cmd_format: cmd_format.o
	$(CC) $(CFLAGS) $^ -o $@
