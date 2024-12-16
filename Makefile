# ==================================================== #
#          C O M P I L A T I O N   S E T U P           #
# ==================================================== #
#            defines the compiler and flags            #
# ---------------------------------------------------- #

CC = gcc
CFLAGS = -g -W -Wall -pedantic -std=c99 -O3
LEX = lex
YACC = yacc

ARGS = $(filter-out $@,$(MAKECMDGOALS))

BIN_DIR = src/bin
INCLUDE_DIR = src/parser


# ==================================================== #
#         T A R G E T S   D E C L A R A T I O N        #
# ==================================================== #
# 		   defines the targets of the makefile         #
# ---------------------------------------------------- #

COMPILER_RULES = lexer parser
INTERPRETER_RULES = tex yax
GRAMMAR = lexer parser
PARSER = parser.o
SEMANTIC_CHECKS = assignment_validation.o print_validation.o input_validation.o func_proc_validation.o argument_validation.o format_specifiers.o condition_validation.o scope_validation.o type_validation.o variable_validation.o
SYMBOL_TABLE = declaration_table.o name_association.o representation_table.o hash_table.o lexeme_table.o utility.o
TABLE_MANAGEMENT = variable_manager.o array_manager.o func_proc_manager.o structure_manager.o
TYPE_SYSTEM = structure_resolution.o func_proc_resolution.o array_resolution.o expression_resolution.o condition_resolution.o array_access_format.o expression_format.o func_proc_format.o struct_access_format.o
DATA = region_table.o region_stack.o
AST = ast.o lcrs.o
UTILS = stack.o errors.o scope_tracker.o
VIRTUAL_MACHINE = stack_management.o execution.o vm_cell.o interpreter.o


# ==================================================== #
# 					  R U L E S                        #
# ==================================================== #

all: compiler vm simple-clean

install: sudo apt install flex bison

compiler: compiler-rule simple-clean

vm: virtual-machine-rule simple-clean

# ==================================================== #
# 					  T E S T S                    	   #
# ==================================================== #

test: compiler
	PYTHONPATH=tests python3 tests/regression/main.py $(ARGS)


# ---------------------------------------------------- #
#                        COMPILER 					   #
# ---------------------------------------------------- #

compiler-rule: $(COMPILER_RULES) $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS) $(SEMANTIC_CHECKS) $(TYPE_SYSTEM) $(VIRTUAL_MACHINE)
	$(CC) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(PARSER) $(SYMBOL_TABLE) $(TABLE_MANAGEMENT) $(DATA) $(AST) $(UTILS) $(SEMANTIC_CHECKS) $(TYPE_SYSTEM) $(VIRTUAL_MACHINE) -I$(INCLUDE_DIR) -o compiler.exe


# ----------- #
# LEXER
# ----------- #

lexer: src/lexer/lexer.l 
	$(LEX) -o $(BIN_DIR)/lex.yy.c src/lexer/lexer.l


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


# Condition checks
# ----------------
condition_validation.o: src/semantic_analysis/condition_checks/condition_validation.c
	$(CC) -c src/semantic_analysis/condition_checks/condition_validation.c


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

name_association.o: src/symbol_table/declaration/name_association.c
	$(CC) -c src/symbol_table/declaration/name_association.c

representation_table.o: src/symbol_table/representation/representation_table.c
	$(CC) -c src/symbol_table/representation/representation_table.c

lexeme_table.o: src/symbol_table/lexeme/lexeme_table.c
	$(CC) -c src/symbol_table/lexeme/lexeme_table.c

utility.o: src/symbol_table/utility.c
	$(CC) -c src/symbol_table/utility.c

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

# Type inference
# --------------
structure_resolution.o: src/type_system/type_inference/structure_resolution.c
	$(CC) -c src/type_system/type_inference/structure_resolution.c

func_proc_resolution.o: src/type_system/type_inference/func_proc_resolution.c
	$(CC) -c src/type_system/type_inference/func_proc_resolution.c

array_resolution.o: src/type_system/type_inference/array_resolution.c
	$(CC) -c src/type_system/type_inference/array_resolution.c

expression_resolution.o: src/type_system/type_inference/expression_resolution.c
	$(CC) -c src/type_system/type_inference/expression_resolution.c

condition_resolution.o: src/type_system/type_inference/condition_resolution.c
	$(CC) -c src/type_system/type_inference/condition_resolution.c

# Formating
# ---------

array_access_format.o: src/type_system/format/array_access_format.c
	$(CC) -c src/type_system/format/array_access_format.c

expression_format.o: src/type_system/format/expression_format.c
	$(CC) -c src/type_system/format/expression_format.c

func_proc_format.o: src/type_system/format/func_proc_format.c
	$(CC) -c src/type_system/format/func_proc_format.c

struct_access_format.o: src/type_system/format/struct_access_format.c
	$(CC) -c src/type_system/format/struct_access_format.c

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



# ---------------------------------------------------- #
#                I N T E R P R E T E R                 #
# ---------------------------------------------------- #

virtual-machine-rule: $(INTERPRETER_RULES) $(PARSER) $(SYMBOL_TABLE) $(DATA) $(AST) $(UTILS) $(VIRTUAL_MACHINE) 
	$(CC) $(BIN_DIR)/lex.yy.c $(BIN_DIR)/y.tab.c $(PARSER) $(SYMBOL_TABLE) $(DATA) $(AST) $(UTILS) $(VIRTUAL_MACHINE) -I$(INCLUDE_DIR) -o interpreter.exe

tex: src/lexer/interpreter_lexer.l
	$(LEX) -o $(BIN_DIR)/lex.yy.c src/lexer/interpreter_lexer.l

yax: src/parser/interpreter_grammar.y
	$(YACC) -d -Wcounterexamples -o $(BIN_DIR)/y.tab.c src/parser/interpreter_grammar.y

stack_management.o: src/virtual_machine/stack_management/stack_management.c
	$(CC) -c src/virtual_machine/stack_management/stack_management.c

execution.o: src/virtual_machine/core/execution.c
	$(CC) -c src/virtual_machine/core/execution.c

vm_cell.o: src/virtual_machine/core/vm_cell.c
	$(CC) -c src/virtual_machine/core/vm_cell.c

interpreter.o: src/virtual_machine/interpreter/interpreter.c
	$(CC) -c src/virtual_machine/interpreter/interpreter.c

# ==================================================== #
#                        C L E A N                     #
# ==================================================== #
# 			clean generates the .o files 			   #			
# ---------------------------------------------------- #

simple-clean:
	find . -type f -name "*.o" -exec rm -f {} \;

clean: simple-clean
	rm -f *.exe $(BIN_DIR)/*.tab.c $(BIN_DIR)/*.tab.h $(BIN_DIR)/lex.yy.c && \
	rm -rf tests/**/__pycache__
