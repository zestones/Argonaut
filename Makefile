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
SEMANTIC_CHECKS = assignment_validation.o print_validation.o input_validation.o func_proc_validation.o argument_validation.o format_specifiers.o scope_validation.o type_validation.o variable_validation.o
SYMBOL_TABLE = declaration_table.o representation_table.o hash_table.o
TABLE_MANAGEMENT = variable_manager.o array_manager.o func_proc_manager.o structure_manager.o
TYPE_SYSTEM = structure_resolution.o func_proc_resolution.o array_resolution.o expression_resolution.o
DATA = region_table.o region_stack.o
AST = ast.o lcrs.o
UTILS = stack.o errors.o scope_tracker.o


all: compilateur simple-clean

install: sudo apt install flex bison


# ---------------------------------------------------- #
#                        COMPILER 					   #
# ---------------------------------------------------- #

compilateur: $(GRAMMAR) $(LEXER) $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS) $(SEMANTIC_CHECKS) $(TYPE_SYSTEM)
	$(CC) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(LEXER) $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS) $(SEMANTIC_CHECKS) $(TYPE_SYSTEM) -I$(INCLUDE_DIR) -o compilateur.exe


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

parser: src/parser/grammar.y 
	$(YACC) -d -Wcounterexamples -b $(BIN_DIR)/y src/parser/grammar.y

parser.o: src/parser/parser.c
	$(CC) -I$(BIN_DIR) -I$(INCLUDE_DIR) -c src/parser/parser.c


# --------------- #
# SEMANTIC CHECKS
# --------------- #

# Assignment checks
# -----------------
assignment_validation.o: src/semantic_analysis/assignment_checks/assignment_validation.c
	$(CC) -c src/semantic_analysis/assignment_checks/assignment_validation.c


# Function and procedure checks
# -----------------------------
print_validation.o: src/semantic_analysis/function_checks/predefined/print_validation.c
	$(CC) -c src/semantic_analysis/function_checks/predefined/print_validation.c

input_validation.o: src/semantic_analysis/function_checks/predefined/input_validation.c
	$(CC) -c src/semantic_analysis/function_checks/predefined/input_validation.c

func_proc_validation.o: src/semantic_analysis/function_checks/function_validations/func_proc_validation.c
	$(CC) -c src/semantic_analysis/function_checks/function_validations/func_proc_validation.c

argument_validation.o: src/semantic_analysis/function_checks/common/argument_validation.c
	$(CC) -c src/semantic_analysis/function_checks/common/argument_validation.c

format_specifiers.o: src/semantic_analysis/function_checks/common/format_specifiers.c
	$(CC) -c src/semantic_analysis/function_checks/common/format_specifiers.c

# Scope checks
# ------------
scope_validation.o: src/semantic_analysis/scope_checks/scope_validation.c
	$(CC) -c src/semantic_analysis/scope_checks/scope_validation.c


# Type checks
# -----------
type_validation.o: src/semantic_analysis/type_checks/type_validation.c
	$(CC) -c src/semantic_analysis/type_checks/type_validation.c


# Variable checks
# ---------------
variable_validation.o: src/semantic_analysis/variable_checks/variable_validation.c
	$(CC) -c src/semantic_analysis/variable_checks/variable_validation.c


# ----------- #
# SYMBOL TABLE
# ----------- #

hash_table.o: src/symbol_table/hash/hash_table.c
	$(CC) -c src/symbol_table/hash/hash_table.c

declaration_table.o: src/symbol_table/declaration/declaration_table.c
	$(CC) -c src/symbol_table/declaration/declaration_table.c

representation_table.o: src/symbol_table/representation/representation_table.c
	$(CC) -c src/symbol_table/representation/representation_table.c


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
# TYPE SYSTEM
# ----------- #

structure_resolution.o: src/type_system/structure_resolution.c
	$(CC) -c src/type_system/structure_resolution.c

func_proc_resolution.o: src/type_system/func_proc_resolution.c
	$(CC) -c src/type_system/func_proc_resolution.c

array_resolution.o: src/type_system/array_resolution.c
	$(CC) -c src/type_system/array_resolution.c

expression_resolution.o: src/type_system/expression_resolution.c
	$(CC) -c src/type_system/expression_resolution.c

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
