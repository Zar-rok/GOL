CC=g++
CFLAGS=-Wall -g -std=c++11

LIBSFML=-lsfml-graphics -lsfml-window -lsfml-system

VALFLAGS=--leak-check=full --track-origins=yes

TARGET=gol

all: ${TARGET}

${TARGET}: gol.cpp
	${CC} ${CFLAGS} -o $@ $< ${LIBSFML}

clean:
	@rm -f *.o

mrproper: clean
	@rm -f ${TARGET}

l: ${TARGET}
	@./${TARGET}

v: ${TARGET}
	valgrind ${VALFLAGS} ./${TARGET} > tmp.log 2>&1
