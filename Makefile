#compile nqueens (+ GDB data)

CC = g++
CFLAGS = -std=c++11 -g -Iutil
OUT = dungeon
CLEANTXT = *.o *.a *.exe
SRC = launcher.cpp generators.cpp dungeon.cpp console.cpp\
 algo.cpp file.cpp entity.cpp game.cpp _win_unix.cpp

MACROS = -D=__CYGWIN

ifeq ($(shell uname), Linux)
	RUNPATH = $(OUT)
else
#windows
	RUNPATH = $(OUT).exe

endif

all:
	$(CC) $(SRC) $(CFLAGS) $(MACROS) -o $(OUT) -lncurses

#clean extensions, allow for .exe or unix name convention
.PHONY: clean
clean:
	rm -f $@ $(CLEANTXT) $(RUNPATH)

.PHONY: run
run:
	./$(RUNPATH) --nummon 0
		

