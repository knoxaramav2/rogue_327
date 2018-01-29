#compile nqueens (+ GDB data)

CC = gcc
CFLAGS = -std=c11 -g
OUT = dungeon
CLEANTXT = *.o *.a *.exe
SRC = dungeon.c

ifeq ($(shell uname), Linux)
	RUNPATH = $(OUT)
else
#windows
	RUNPATH = $(OUT).exe
endif

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

#clean extensions, allow for .exe or unix name convention
.PHONY: clean
clean:
	rm -f $@ $(CLEANTXT) $(RUNPATH)

.PHONY: run
run:
	./$(RUNPATH)
		

