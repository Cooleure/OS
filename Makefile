.PHONY:all

TARGETS=cmd_format installer osFromScratch
INSTALLER=installer

all: $(TARGETS)

clean:
	@rm -f *.o *~ $(TARGETS) *.d

CC=gcc
CFLAGS=-Wall -std=c99
sources=$(wildcard *.c)

# compile
%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

osFromScratch: layer1.o layer2.o main.o user_interface.o
	$(CC) $(CFLAGS) $^ -o $@

cmd_format: cmd_format.o
	$(CC) $(CFLAGS) $^ -o $@

installer: installer.o layer1.o layer2.o
	$(CC) $(CFLAGS) $^ -o $@
