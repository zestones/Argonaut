# ==================================================== #
#          C O M P I L A T I O N   S E T U P           #
# ==================================================== #
#            defines the compiler and flags            #
# ---------------------------------------------------- #

CC = gcc
CFLAGS = -g -W -Wall -pedantic -std=c99 -O3
LEX = lex
YACC = yacc

BIN_DIR = src/bin
INCLUDE_DIR = src/parser


# ==================================================== #
#         T A R G E T S   D E C L A R A T I O N        #
# ==================================================== #
# 		   defines the targets of the makefile         #
# ---------------------------------------------------- #

GRAMMAR = lexer parser
LEXER = lexeme_table.o 
PARSER = parser.o
SYMBOL_TABLE = declaration_table.o representation_table.o hash_table.o
TABLE_MANAGEMENT = variable_manager.o array_manager.o func_proc_manager.o structure_manager.o
DATA = region_table.o region_stack.o
AST = ast.o lcrs.o
UTILS = stack.o errors.o validation.o scope_tracker.o


all: compilateur simple-clean

install: sudo apt install flex bison


# ---------------------------------------------------- #
#                        COMPILER 					   #
# ---------------------------------------------------- #

compilateur: $(GRAMMAR) $(LEXER) $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS)  
	$(CC) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(LEXER) $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS) -I$(INCLUDE_DIR) -o compilateur.exe


# ----------- #
# LEXER
# ----------- #

lexer: src/lexer/lexer.l 
	$(LEX) -o $(BIN_DIR)/lex.yy.c src/lexer/lexer.l

lexeme_table.o: src/lexer/lexeme_table.c
	$(CC) -c src/lexer/lexeme_table.c


# ----------- #
# PARSER
# ----------- #

parser: src/parser/yacc_parser.y 
	# ./src/utils/yacc.sh
	$(YACC) -d -Wcounterexamples -b $(BIN_DIR)/y src/parser/yacc_parser.y

parser.o: src/parser/parser.c
	$(CC) -I$(BIN_DIR) -I$(INCLUDE_DIR) -c src/parser/parser.c


# ----------- #
# SYMBOL TABLE
# ----------- #

hash_table.o: src/symbol_table/hash_table.c
	$(CC) -c src/symbol_table/hash_table.c

declaration_table.o: src/symbol_table/declaration_table.c
	$(CC) -c src/symbol_table/declaration_table.c

representation_table.o: src/symbol_table/representation_table.c
	$(CC) -c src/symbol_table/representation_table.c


# ---------------- #
# TABLE MANAGEMENT
# ---------------- #

variable_manager.o: src/table_management/variable_manager.c
	$(CC) -c src/table_management/variable_manager.c

array_manager.o: src/table_management/array_manager.c
	$(CC) -c src/table_management/array_manager.c

func_proc_manager.o: src/table_management/func_proc_manager.c
	$(CC) -c src/table_management/func_proc_manager.c

structure_manager.o: src/table_management/structure_manager.c
	$(CC) -c src/table_management/structure_manager.c


# ----------- #
# DATA
# ----------- #

region_table.o: src/data/region_table.c
	$(CC) -c src/data/region_table.c

region_stack.o: src/data/region_stack.c
	$(CC) -c src/data/region_stack.c

# ----------- #
# AST
# ----------- #

ast.o: src/ast/ast.c
	$(CC) -c src/ast/ast.c

lcrs.o: src/ast/lcrs.c
	$(CC) -c src/ast/lcrs.c

# ----------- #
# UTILS
# ----------- #

stack.o: src/utils/stack.c
	$(CC) -c src/utils/stack.c

errors.o: src/utils/errors.c
	$(CC) -c src/utils/errors.c

validation.o: src/utils/validation.c
	$(CC) -c src/utils/validation.c

scope_tracker.o: src/utils/scope_tracker.c
	$(CC) -c src/utils/scope_tracker.c

# ==================================================== #
#                        C L E A N                     #
# ==================================================== #
# 			clean generates the .o files 			   #			
# ---------------------------------------------------- #

simple-clean:
	rm -f *.o

clean: simple-clean
	rm -f *.exe $(BIN_DIR)/*.tab.c $(BIN_DIR)/*.tab.h $(BIN_DIR)/lex.yy.c
