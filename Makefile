.PHONY:all

TARGETS=cmd_format installer osFromScratch
INSTALLER=installer

CC=gcc
CFLAGS=-Wall -std=c99
sources=$(wildcard *.c)

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

all: $(TARGETS)

clean:
	@rm -f *.o *~ $(TARGETS) *.d

# compile
%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

osFromScratch: layer1.o layer2.o layer5.o main.o user_interface.o
	$(CC) $(CFLAGS) $^ -o $@

cmd_format: cmd_format.o
	$(CC) $(CFLAGS) $^ -o $@

installer: installer.o layer1.o layer2.o
	$(CC) $(CFLAGS) $^ -o $@
