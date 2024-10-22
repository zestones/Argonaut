CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3
LEX = lex
YACC = yacc

BIN_DIR = src/bin
INCLUDE_DIR = src/parser

# -- MAIN -- #
all: compilateur simple-clean

install: sudo apt install flex bison

compilateur: lexer parser parser.o hash.o lexeme_table.o declaration_table.o
	$(CC) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c parser.o hash.o lexeme_table.o declaration_table.o -I$(INCLUDE_DIR) -o compilateur.exe

lexer: src/lexer/lexer.l 
	$(LEX) -o $(BIN_DIR)/lex.yy.c src/lexer/lexer.l

parser: src/parser/yacc_parser.y 
	./src/utils/yacc.sh
	$(YACC) -d -Wcounterexamples -b $(BIN_DIR)/y src/parser/yacc_parser.y

parser.o: src/parser/parser.c
	$(CC) -I$(BIN_DIR) -I$(INCLUDE_DIR) -c src/parser/parser.c

lexeme_table.o: src/lexer/lexeme_table.c
	$(CC) -c src/lexer/lexeme_table.c

declaration_table.o: src/parser/declaration_table/declaration_table.c
	$(CC) -c src/parser/declaration_table/declaration_table.c

# -- UTILS -- #

hash.o: src/utils/hash.c
	$(CC) -c src/utils/hash.c

utils:

simple-clean:
	rm -f *.o

clean: simple-clean
	rm -f *.exe $(BIN_DIR)/*.tab.c $(BIN_DIR)/*.tab.h $(BIN_DIR)/lex.yy.c
