# ==============================================================================
#  Argonaut — build system
#
#  Usage
#    make                        build argoc + argov (CONFIG=debug) and copy them to bin/
#    make CONFIG=release         optimised build with hardening (-O2, FORTIFY, stack protector)
#    make CONFIG=sanitize        AddressSanitizer + UndefinedBehaviorSanitizer build
#    make CONFIG=msan CC=clang   MemorySanitizer build (clang only)
#    make CONFIG=coverage        instrumented build used by `make coverage`
#    make check                  unit tests + language tests + IR tests + regression suite
#    make lint                   formatting, banned functions and warnings ratchets
#    make lint-full              lint plus cppcheck, clang-tidy and gcc -fanalyzer (target state)
#    make coverage               run the suite with coverage instrumentation and report
#    make bench                  release build vs the Python/C references, and vs the baseline
#    make fuzz                   fuzz argoc with AFL++ (needs afl-fuzz)
#    make WERROR=1               warnings are errors (the CI "warnings-as-errors" job)
#    make V=1                    verbose command echo
#    make -j                     parallel builds are supported
#
#  Layout
#    build/gen/{compiler,vm}/    flex/bison outputs (never committed)
#    build/<CONFIG>/obj/         objects + automatic dependency files
#    build/<CONFIG>/argoc|argov  binaries of that configuration
#    bin/                        copies of the last built configuration (convenience)
# ==============================================================================

CC       ?= gcc
LEX      ?= flex
YACC     ?= bison
PYTHON   ?= python3
CONFIG   ?= debug
WERROR   ?= 0
V        ?= 0
TEST_JOBS ?= 2

# ---- strict warning set (kept strict on purpose: see README.md, Development) ----
CSTD     ?= -std=gnu17
WARNINGS := -Wall -Wextra -Wpedantic \
            -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition \
            -Wshadow -Wvla -Wformat=2 -Wundef -Wwrite-strings -Wpointer-arith -Wcast-qual
ifeq ($(WERROR),1)
WARNINGS += -Werror
endif
CFLAGS_EXTRA ?=

# ---- configurations ----------------------------------------------------------
ifeq ($(CONFIG),debug)
  CFLAGS_CONFIG  := -O0 -g3 -DARGO_DEBUG
  LDFLAGS_CONFIG :=
else ifeq ($(CONFIG),release)
  CFLAGS_CONFIG  := -O2 -g -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong
  LDFLAGS_CONFIG :=
else ifeq ($(CONFIG),sanitize)
  CFLAGS_CONFIG  := -O1 -g -DARGO_DEBUG -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer
  LDFLAGS_CONFIG := -fsanitize=address,undefined
else ifeq ($(CONFIG),msan)
  CFLAGS_CONFIG  := -O1 -g -DARGO_DEBUG -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer
  LDFLAGS_CONFIG := -fsanitize=memory
else ifeq ($(CONFIG),coverage)
  CFLAGS_CONFIG  := -O0 -g -DARGO_DEBUG --coverage
  LDFLAGS_CONFIG := --coverage
else
  $(error Unknown CONFIG '$(CONFIG)' — use debug, release, sanitize, msan or coverage)
endif

# ---- layout ------------------------------------------------------------------
SRC_DIR   := src
TEST_DIR  := tests
EXAMPLE_TEST_DIR := examples/interpretation
BUILD     := build
GEN_DIR   := $(BUILD)/gen
OUT_DIR   := $(BUILD)/$(CONFIG)
OBJ_DIR   := $(OUT_DIR)/obj
BIN_DIR   := bin

SRC_C     := $(shell find $(SRC_DIR) -name '*.c' ! -path '$(SRC_DIR)/bin/*' | sort)
OBJ       := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_C))

COMPILER_GEN_DIR := $(GEN_DIR)/compiler
VM_GEN_DIR       := $(GEN_DIR)/vm
COMPILER_OBJ     := $(OBJ_DIR)/gen/compiler/lex.yy.o $(OBJ_DIR)/gen/compiler/y.tab.o
VM_OBJ           := $(OBJ_DIR)/gen/vm/lex.yy.o $(OBJ_DIR)/gen/vm/y.tab.o

UNIT_SRC  := $(sort $(wildcard $(TEST_DIR)/unit/*.c))
UNIT_OBJ  := $(patsubst $(TEST_DIR)/unit/%.c,$(OBJ_DIR)/tests/unit/%.o,$(UNIT_SRC))
# parser.o needs yyparse (a generated front-end); unit tests link everything else.
UNIT_LINK := $(filter-out $(OBJ_DIR)/parser/parser.o,$(OBJ))

ARGOC := $(OUT_DIR)/argoc
ARGOV := $(OUT_DIR)/argov
UNIT  := $(OUT_DIR)/unit_tests

CPPFLAGS := -I$(SRC_DIR)/parser -I$(COMPILER_GEN_DIR) -MMD -MP
CFLAGS   := $(CSTD) $(WARNINGS) $(CFLAGS_CONFIG) $(CFLAGS_EXTRA)
LDFLAGS  := $(LDFLAGS_CONFIG)

# Expected-failure list of this configuration (the "ratchet", see tests/README.md).
# coverage shares debug's list (same flags); msan and valgrind have their own.
XFAIL_CONFIG := $(if $(filter coverage,$(CONFIG)),debug,$(CONFIG))
XFAIL_ARGS   := --xfail $(TEST_DIR)/xfail/$(XFAIL_CONFIG).txt

ifeq ($(V),1)
  Q :=
else
  Q := @
endif

# Sanitizer runtime options (override from the environment). Leak detection is a separate
# ratchet (`make check-leaks`, tests/xfail/leaks.txt) so that the ASan/UBSan gate stays
# focused on memory errors and undefined behaviour.
export ASAN_OPTIONS  ?= detect_stack_use_after_return=1:detect_leaks=0:strict_string_checks=1
export UBSAN_OPTIONS ?= print_stacktrace=1
export MSAN_OPTIONS  ?= print_stats=0

.DELETE_ON_ERROR:
.PHONY: all compiler vm test check check-unit check-lang check-regression check-leaks check-valgrind doctests \
        lint lint-full format format-check banned-check warnings-check warnings-baseline analyze cppcheck tidy \
        coverage bench fuzz fuzz-vm docs clean distclean deps-apt extension-install extension-uninstall help

all: $(BIN_DIR)/argoc $(BIN_DIR)/argov
compiler: $(BIN_DIR)/argoc
vm: $(BIN_DIR)/argov

$(BIN_DIR)/argoc: $(ARGOC) | $(BIN_DIR)
	$(Q)cp $< $@
$(BIN_DIR)/argov: $(ARGOV) | $(BIN_DIR)
	$(Q)cp $< $@

$(BIN_DIR) $(COMPILER_GEN_DIR) $(VM_GEN_DIR):
	$(Q)mkdir -p $@

# ---- generated front-ends (bison writes .c and .h together: grouped target) ----
$(COMPILER_GEN_DIR)/y.tab.c $(COMPILER_GEN_DIR)/y.tab.h &: $(SRC_DIR)/parser/grammar.y | $(COMPILER_GEN_DIR)
	@echo "  YACC    $<"
	$(Q)$(YACC) -Wall -Wcounterexamples -d -o $(COMPILER_GEN_DIR)/y.tab.c $<

$(COMPILER_GEN_DIR)/lex.yy.c: $(SRC_DIR)/lexer/lexer.l $(COMPILER_GEN_DIR)/y.tab.h | $(COMPILER_GEN_DIR)
	@echo "  LEX     $<"
	$(Q)$(LEX) -o $@ $<

$(VM_GEN_DIR)/y.tab.c $(VM_GEN_DIR)/y.tab.h &: $(SRC_DIR)/parser/interpreter_grammar.y | $(VM_GEN_DIR)
	@echo "  YACC    $<"
	$(Q)$(YACC) -Wall -Wcounterexamples -d -o $(VM_GEN_DIR)/y.tab.c $<

$(VM_GEN_DIR)/lex.yy.c: $(SRC_DIR)/lexer/interpreter_lexer.l $(VM_GEN_DIR)/y.tab.h | $(VM_GEN_DIR)
	@echo "  LEX     $<"
	$(Q)$(LEX) -o $@ $<

# ---- objects -----------------------------------------------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/parser/parser.o: $(COMPILER_GEN_DIR)/y.tab.h

$(OBJ_DIR)/gen/compiler/%.o: $(COMPILER_GEN_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) -I$(SRC_DIR)/parser -I$(COMPILER_GEN_DIR) -MMD -MP $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/gen/vm/%.o: $(VM_GEN_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) -I$(SRC_DIR)/parser -I$(VM_GEN_DIR) -MMD -MP $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/unit/%.o: $(TEST_DIR)/unit/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) $(CPPFLAGS) -I$(SRC_DIR) $(CFLAGS) -c $< -o $@

# ---- binaries ----------------------------------------------------------------
$(ARGOC): $(OBJ) $(COMPILER_OBJ)
	@echo "  LINK    $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(ARGOV): $(OBJ) $(VM_OBJ)
	@echo "  LINK    $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(UNIT): $(UNIT_OBJ) $(UNIT_LINK)
	@echo "  LINK    $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

-include $(OBJ:.o=.d) $(COMPILER_OBJ:.o=.d) $(VM_OBJ:.o=.d) $(UNIT_OBJ:.o=.d)

# ---- tests -------------------------------------------------------------------
test: check
check: all check-unit check-lang check-regression

check-unit: $(UNIT)
	@echo "==> unit tests ($(CONFIG))"
	$(Q)$(UNIT)

doctests: tools/extract_doctests.py
	$(Q)$(PYTHON) tools/extract_doctests.py --out $(BUILD)/doctests README.md wiki

check-lang: $(ARGOC) $(ARGOV) doctests
	@echo "==> language, IR, examples and documentation tests ($(CONFIG))"
	$(Q)$(PYTHON) $(TEST_DIR)/run_tests.py --argoc $(ARGOC) --argov $(ARGOV) $(XFAIL_ARGS) \
	    --jobs $(TEST_JOBS) --junit $(OUT_DIR)/junit-lang.xml \
	    $(TEST_DIR)/lang $(TEST_DIR)/ir $(EXAMPLE_TEST_DIR) $(BUILD)/doctests

# The diagnostics suite compares exact text: sanitizer reports would pollute it, so it runs on
# the plain configurations only (the language suite covers the instrumented builds).
check-regression: $(ARGOC)
	@echo "==> compiler diagnostics regression suite ($(CONFIG))"
	$(Q)if [ "$(CONFIG)" = sanitize ] || [ "$(CONFIG)" = msan ]; then \
	    echo "skipped on the $(CONFIG) configuration (exact-text suite)"; \
	else ARGOC=$(abspath $(ARGOC)) PYTHONPATH=$(TEST_DIR) $(PYTHON) $(TEST_DIR)/regression/main.py --run; fi

# LeakSanitizer ratchet: the language suite with leak detection on (sanitize configuration only).
check-leaks: $(ARGOC) $(ARGOV) doctests
	@echo "==> language tests with LeakSanitizer ($(CONFIG))"
	$(Q)test "$(CONFIG)" = "sanitize" || { echo "check-leaks needs CONFIG=sanitize"; exit 1; }
	$(Q)ASAN_OPTIONS="detect_stack_use_after_return=1:detect_leaks=1:strict_string_checks=1" \
	    $(PYTHON) $(TEST_DIR)/run_tests.py --argoc $(ARGOC) --argov $(ARGOV) \
	    --xfail $(TEST_DIR)/xfail/leaks.txt --jobs $(TEST_JOBS) --junit $(OUT_DIR)/junit-leaks.xml \
	    $(TEST_DIR)/lang $(TEST_DIR)/ir $(EXAMPLE_TEST_DIR) $(BUILD)/doctests

# Valgrind memcheck over the language suite (manual Extended checks workflow in CI).
VALGRIND := valgrind --error-exitcode=99 --leak-check=full --errors-for-leak-kinds=definite -q
check-valgrind: $(ARGOC) $(ARGOV) doctests
	@echo "==> language tests under valgrind ($(CONFIG))"
	$(Q)$(PYTHON) $(TEST_DIR)/run_tests.py --argoc $(ARGOC) --argov $(ARGOV) \
	    --xfail $(TEST_DIR)/xfail/valgrind.txt --wrapper "$(VALGRIND)" --timeout 120 --jobs $(TEST_JOBS) \
	    $(TEST_DIR)/lang $(TEST_DIR)/ir $(EXAMPLE_TEST_DIR) $(BUILD)/doctests

# ---- quality gates -------------------------------------------------------------
lint: format-check banned-check warnings-check

# Target state: deliberately red until the static-analysis debt is paid. Run the analyzers
# sequentially because some of them rebuild the same generated files and objects.
lint-full: lint
	$(Q)$(MAKE) --no-print-directory cppcheck
	$(Q)$(MAKE) --no-print-directory tidy
	$(Q)$(MAKE) --no-print-directory analyze

format:
	$(Q)tools/format.sh --apply

format-check:
	$(Q)tools/format.sh --check

banned-check:
	$(Q)$(PYTHON) tools/check_banned.py

# Rebuild everything with the strict flags and make sure the warning count did not grow.
warnings-check:
	@echo "==> warnings ratchet (full rebuild, $(CONFIG))"
	$(Q)$(MAKE) --no-print-directory -B V=1 $(ARGOC) $(ARGOV) $(UNIT) 2>&1 | $(PYTHON) tools/check_warnings.py

warnings-baseline:
	@echo "==> rewriting the warnings baseline (full rebuild, $(CONFIG))"
	$(Q)$(MAKE) --no-print-directory -B V=1 $(ARGOC) $(ARGOV) $(UNIT) 2>&1 | $(PYTHON) tools/check_warnings.py --update

# GCC static analyzer: any -Wanalyzer-* finding fails the gate.
analyze:
	@echo "==> gcc -fanalyzer"
	$(Q)$(MAKE) --no-print-directory -B V=1 CONFIG=$(CONFIG) CFLAGS_EXTRA="-fanalyzer" $(ARGOC) $(ARGOV) 2>&1 \
	    | $(PYTHON) tools/check_warnings.py --analyzer

cppcheck:
	$(Q)if command -v cppcheck >/dev/null 2>&1; then \
	    echo "==> cppcheck"; \
	    cppcheck --std=c17 --language=c --enable=warning,performance,portability --inline-suppr \
	        --error-exitcode=1 --suppress=missingIncludeSystem -I $(SRC_DIR)/parser -I $(COMPILER_GEN_DIR) \
	        --quiet $(SRC_DIR); \
	else echo "cppcheck: not installed, skipped"; fi

tidy:
	$(Q)if command -v clang-tidy >/dev/null 2>&1; then \
	    echo "==> clang-tidy"; \
	    clang-tidy --quiet $(SRC_C) -- -I$(SRC_DIR)/parser -I$(COMPILER_GEN_DIR) $(CSTD); \
	else echo "clang-tidy: not installed, skipped"; fi

# ---- coverage, benchmarks, fuzzing --------------------------------------------
coverage:
	$(Q)$(MAKE) --no-print-directory CONFIG=coverage check || true
	$(Q)$(PYTHON) tools/coverage_report.py --build $(BUILD)/coverage --src $(SRC_DIR)

# always the release build: comparing a -O0 argov with C -O2 would be meaningless
bench:
	$(Q)$(MAKE) --no-print-directory CONFIG=release $(BUILD)/release/argoc $(BUILD)/release/argov
	$(Q)$(PYTHON) $(TEST_DIR)/bench/run_bench.py --argoc $(BUILD)/release/argoc --argov $(BUILD)/release/argov --cc $(CC) --check

FUZZ_TIME ?= 600
FUZZ_CC   ?= afl-clang-fast
fuzz:
	$(Q)command -v afl-fuzz >/dev/null 2>&1 || { echo "afl-fuzz not found: install AFL++ (apt install afl++)"; exit 1; }
	$(Q)$(MAKE) --no-print-directory CONFIG=debug CC=$(FUZZ_CC) BUILD=$(BUILD)/fuzz $(BUILD)/fuzz/debug/argoc
	$(Q)mkdir -p $(BUILD)/fuzz/corpus $(BUILD)/fuzz/out
	$(Q)cp $(wildcard examples/interpretation/functional/*/*.arn) $(BUILD)/fuzz/corpus/ 2>/dev/null || true
	$(Q)AFL_SKIP_CPUFREQ=1 AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 afl-fuzz -V $(FUZZ_TIME) -t 2000 \
	    -x $(TEST_DIR)/fuzz/argonaut.dict -i $(BUILD)/fuzz/corpus -o $(BUILD)/fuzz/out \
	    -- $(BUILD)/fuzz/debug/argoc -a @@ -o /dev/null

fuzz-vm:
	$(Q)command -v afl-fuzz >/dev/null 2>&1 || { echo "afl-fuzz not found: install AFL++ (apt install afl++)"; exit 1; }
	$(Q)$(MAKE) --no-print-directory CONFIG=debug CC=$(FUZZ_CC) BUILD=$(BUILD)/fuzz $(BUILD)/fuzz/debug/argov
	$(Q)$(PYTHON) $(TEST_DIR)/fuzz/make_ir_corpus.py --argoc $(ARGOC) --out $(BUILD)/fuzz/ir-corpus
	$(Q)mkdir -p $(BUILD)/fuzz/out-vm
	$(Q)AFL_SKIP_CPUFREQ=1 AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 afl-fuzz -V $(FUZZ_TIME) -t 2000 \
	    -i $(BUILD)/fuzz/ir-corpus -o $(BUILD)/fuzz/out-vm -- $(BUILD)/fuzz/debug/argov -a @@

# ---- documentation -------------------------------------------------------------
docs:
	doxygen Doxyfile

# ---- housekeeping --------------------------------------------------------------
clean:
	$(Q)rm -rf $(BUILD) $(BIN_DIR)/argoc $(BIN_DIR)/argov log.txt

distclean: clean
	$(Q)rm -rf docs/html examples/compilation/.out/actual_outputs
	$(Q)find . -name __pycache__ -type d -prune -exec rm -rf {} +

deps-apt:
	sudo apt-get install -y build-essential flex bison valgrind cppcheck clang clang-format clang-tidy doxygen graphviz

extension-install:
	@if [ -d ~/.vscode/extensions/ ]; then cp -r extensions/* ~/.vscode/extensions/; fi
	@if [ -d ~/.vscode-server/extensions/ ]; then cp -r extensions/* ~/.vscode-server/extensions/; fi

extension-uninstall:
	rm -rf ~/.vscode/extensions/custom-icons ~/.vscode-server/extensions/custom-icons
	rm -rf ~/.vscode/extensions/syntax-highlighting ~/.vscode-server/extensions/syntax-highlighting

help:
	@sed -n '2,22p' $(MAKEFILE_LIST)
