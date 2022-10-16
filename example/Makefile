CC = gcc
FLAGS = -Wall -Wextra

OBJ_DIR = obj/
SRC_DIR = src/
MAIN = $(SRC_DIR)main.c
OBJ_SOURCE = $(filter-out $(MAIN), $(wildcard $(SRC_DIR)*.c))
OBJ_FILES = $(addprefix $(OBJ_DIR), $(notdir $(OBJ_SOURCE:.c=.o)))

all: $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(FLAGS) -o main $(MAIN) $(OBJ_FILES) libmatrix/lib/libmatrix.a
	@echo "Compiled for production."

$(OBJ_DIR):
	@[ ! -d $(OBJ_DIR) ] && mkdir $(OBJ_DIR) || true

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) -c -o $@ -D SAFE_MATRIX $<

clean:
	rm -rf $(OBJ_DIR)
	rm -f main

setup:
	@[ ! -d libmatrix ] && git clone https://github.com/JacobLinCool/libmatrix.git || true
	@cd libmatrix && git pull && make
	@echo "Setup done."
