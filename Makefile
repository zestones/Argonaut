CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3

all: compilateur IFmple-clean

install: sudo apt install flex bison

compilateur: compilateur-lex compilateur-yacc
	$(CC) lex.yy.c y.tab.c -o compilateur.exe

compilateur-lex: src/compilateur.l
	lex src/compilateur.l

compilateur-yacc: src/compilateur.y
	yacc -d src/compilateur.y

IFmple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: IFmple-clean
	rm -f *.exe