# Compiler commands and flags
CC = gcc
FLAGS = -Wall -Wextra
EXE_FLAGS = -lm
DEV_FLAGS = -g -fsanitize=address -D DEBUG
PROD_FLAGS = -O3

# File and directory declarations
CWD = $(shell pwd)
DIRNAME = $(notdir $(CWD))
SRC_DIR = src/
LIB_DIR = lib/
OBJ_DIR = .obj/
TEST_DIR = .test/
C_FILES := $(wildcard $(SRC_DIR)*.c)
TEST_FILES := $(wildcard $(SRC_DIR)*.test.c)
SRC_FILES := $(filter-out $(TEST_FILES), $(C_FILES))
LIB_FILES := $(filter-out $(EXE_FILES), $(SRC_FILES))
OBJ_FILES := $(addprefix $(OBJ_DIR), $(notdir $(LIB_FILES:.c=.o)))

# DON'T FORGET TO TRY `make help`
all: prod

# `make help`: print this help message
help: print-help

# `make prod`: compile all files for production (optimized)
prod: FLAGS += $(PROD_FLAGS)
prod: prepare $(EXE_FILES:.c=)
prod:
	@[ ! -d $(LIB_DIR) ] && mkdir $(LIB_DIR) || true
	gcc -shared $(FLAGS) -o $(LIB_DIR)libmatrix.so $(OBJ_FILES)
	@echo "Compiled for production."

# `make dev`: compile all files for development (with debug symbols)
dev: FLAGS += $(DEV_FLAGS)
dev: prepare $(EXE_FILES:.c=.dev)

# `make list`: list files in the directory
list:
	@echo "Directory  : $(DIRNAME)"
	@echo "C FILES    : $(C_FILES)"
	@echo "SRC FILES  : $(SRC_FILES)"
	@echo "LIB FILES  : $(LIB_FILES)"
	@echo "TEST FILES : $(TEST_FILES)"
	@echo "OBJECT FILES : $(OBJ_FILES)"

# `make clean`: clean up the workspace
clean: clean-dir clean-dev clean-prod
	@rm -f $(DIRNAME).zip *.pdf
	@echo Cleaned up the workspace.

# `make test`: run all tests
test: prod prepare-test
	@for file in $(addprefix $(TEST_DIR), $(notdir $(TEST_FILES:.c=))); do ./$$file; done
	@echo \\033[92mAll tests passed.\\033[m

# Compile executable targets
%: %.c
	$(CC) $(FLAGS) $(EXE_FLAGS) -o $@ $< $(OBJ_FILES)

%.dev: %.c
	$(CC) $(FLAGS) $(EXE_FLAGS) -o $@ $< $(OBJ_FILES)

# Compile test targets
prepare-test: $(TEST_DIR) $(addprefix $(TEST_DIR), $(notdir $(TEST_FILES:.c=)))

$(TEST_DIR):
	@mkdir -p $@

$(TEST_DIR)%.test: $(SRC_DIR)%.test.c $(OBJ_FILES)
	$(CC) $(FLAGS) -o $@ $< $(OBJ_FILES)

# Compile library object files
prepare: $(OBJ_DIR) $(OBJ_FILES)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(SRC_DIR)%.h
	$(CC) $(FLAGS) -c -o $@ $<

# Clean up the workspace
clean-dir:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(TEST_DIR)
	@rm -rf $(LIB_DIR)

clean-dev:
	@rm -f $(EXE_FILES:.c=.dev)
	@rm -rf *.dSYM

clean-prod:
	@rm -f $(EXE_FILES:.c=)

# Help messages
print-help:
	@echo "Usage: make [target]"
	@echo
	@echo "Targets:"
	@echo "    prod (default) Compile all files for production (optimized)"
	@echo "    dev            Compile all files for development (with debug symbols)"
	@echo "    list           List files in the directory"
	@echo "    clean          Clean up the workspace"
	@echo "    test           Run all tests"
	@echo "    help           Print this help message"

.PHONY: all help prod dev list clean prepare
