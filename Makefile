CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3
LEX = lex
YACC = yacc

all: compilateur simple-clean

install: sudo apt install flex bison

compilateur: lexer parser
	$(CC) lex.yy.c y.tab.c -o compilateur.exe

lexer: src/lexer/lexer.l
	$(LEX) src/lexer/lexer.l

parser: src/parser/combined.y 
	./src/utils/yacc.sh
	$(YACC) -d -Wcounterexamples src/parser/combined.y

simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: simple-clean
	rm -f *.exe