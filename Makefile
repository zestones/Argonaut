# ########################################################
#	     _                                      _        #
#	    / \   _ __ __ _  ___  _ __   __ _ _   _| |_ 	 #
#	   / _ \ | '__/ _` |/ _ \| '_ \ / _` | | | | __|	 #
#	  / ___ \| | | (_| | (_) | | | | (_| | |_| | |_ 	 #
#	 /_/   \_\_|  \__, |\___/|_| |_|\__,_|\__,_|\__|	 #
#	              |___/                             	 #
# ########################################################
#														 #	
# ====================================================== #
#           C O M P I L A T I O N   S E T U P          	 #
# ====================================================== # 
#             defines the compiler and flags             #
# ------------------------------------------------------ #
CC      = gcc
CFLAGS  = -g -Wall -Wextra -pedantic
LEX     = lex
YACC    = yacc

# Directories
SRC_DIR         = src
TESTS_DIR 		= tests
BIN_DIR         = $(SRC_DIR)/bin
LEXER_DIR       = $(SRC_DIR)/lexer
PARSER_DIR      = $(SRC_DIR)/parser
INCLUDE_DIRS    = -I$(PARSER_DIR)
BIN				= bin

COMPILER_RULES      = lexer parser
INTERPRETER_RULES   = vm_lexer vm_parser

# 🛠️ Generated Files from Yacc (Parser)
COMPILER_PARSER_GEN_H     = $(BIN_DIR)/y.tab.h
INTERPRETER_PARSER_GEN_H  = $(BIN_DIR)/y.tab.h

# All object files to be compiled
OTHER_SRC     = $(shell find $(SRC_DIR) -name '*.c' ! -path "$(BIN_DIR)/*")
ALL_OBJ       = $(OTHER_SRC:.c=.o)   

# Executables to generate
EXECUTABLES  = argoc argov 

# ====================================================== #
#                B U I L D  T A R G E T S            	 #
# ====================================================== # 

# All targets to be built
all: compiler vm simple-clean 

install: 
	sudo apt install flex bison
	sudo apt install doxygen
	sudo apt install gdb

extension-install:
	@if [ -d ~/.vscode/extensions/ ]; then \
		cp -r extensions/* ~/.vscode/extensions/; \
	fi
	@if [ -d ~/.vscode-server/extensions/ ]; then \
		cp -r extensions/* ~/.vscode-server/extensions/; \
	fi

docs:
	doxygen Doxyfile

# ====================================================== #
#                     C O M P I L E R                    #
# ====================================================== # 
#  Build the compiler executable from lexer and parser   #
# ------------------------------------------------------ #

compiler: argoc

argoc: $(COMPILER_RULES) $(ALL_OBJ) $(COMPILER_PARSER_GEN_H)
	$(CC) $(CFLAGS) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(ALL_OBJ) $(INCLUDE_DIRS) -o $(BIN)/$@

# Rule to compile .c files into .o object files for the compiler
%.o: %.c $(COMPILER_PARSER_GEN_H)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

lexer: $(SRC_DIR)/lexer/lexer.l
	$(LEX) -o $(BIN_DIR)/lex.yy.c $(SRC_DIR)/lexer/lexer.l

parser: $(SRC_DIR)/parser/grammar.y
	$(YACC) -d -Wcounterexamples -b $(BIN_DIR)/y $(SRC_DIR)/parser/grammar.y


# ====================================================== # 
#               V I R T U A L   M A C H I N E            #
# ====================================================== # 
#     Build the interpreter executable for the VM        #
# ------------------------------------------------------ #

vm: argov

argov: $(INTERPRETER_RULES) $(ALL_OBJ) $(INTERPRETER_PARSER_GEN_H)
	$(CC) $(CFLAGS) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(ALL_OBJ) $(INCLUDE_DIRS) -o $(BIN)/$@

vm_lexer: $(SRC_DIR)/lexer/interpreter_lexer.l
	$(LEX) -o $(BIN_DIR)/lex.yy.c $(SRC_DIR)/lexer/interpreter_lexer.l

vm_parser: $(SRC_DIR)/parser/interpreter_grammar.y
	$(YACC) -d -Wcounterexamples -o $(BIN_DIR)/y.tab.c $(SRC_DIR)/parser/interpreter_grammar.y

# ====================================================== # 
#               T E S T I N G   T A R G E T S            #
# ====================================================== #
#          Running tests to ensure functionality         #
# ------------------------------------------------------ #

ARGS = $(filter-out $@,$(MAKECMDGOALS))

test: compiler simple-clean
	PYTHONPATH=$(TESTS_DIR) python3 tests/regression/main.py $(ARGS)

# ====================================================== # 
#                      C L E A N I N G                   #
# ====================================================== # 
#            Clean object files and executables          #
# ------------------------------------------------------ #

simple-clean:
	find . -type f -name "*.o" -exec rm -f {} \;


clean: simple-clean
	rm -f argoc argov $(BIN_DIR)/*.tab.c $(BIN_DIR)/*.tab.h $(BIN_DIR)/lex.yy.c && \
	rm -rf tests/**/__pycache__
	rm -rf ./.pytest_cache/
	rm -f log.txt

extension-uninstall:
	rm -rf ~/.vscode/extensions/custom-icons
	rm -rf ~/.vscode-server/extensions/custom-icons
	rm -rf ~/.vscode/extensions/syntax-highlighting
	rm -rf ~/.vscode-server/extensions/syntax-highlighting