#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "format.h"
#include "matrices.h"
#include "ui.h"

enum Option {
	OP_LIST = 1,
	OP_CREATE,
	OP_DELETE,
	OP_PRINT,
	OP_PRINT_SPARSE,
	OP_RENAME,
	OP_EDIT,
	OP_TRANSPOSE,
	OP_SCALAR_MULTIPLY,
	OP_ADD,
	OP_MULTIPLY,
	OP_HADAMARD,
	OP_EXP,
	OP_SUBMATRIX,
	OP_READ,
	OP_WRITE,
} Option;

void op_list();
void op_create();
void op_delete();
void op_print();
void op_print_sparse();
void op_rename();
void op_edit();
void op_transpose();
void op_scale();
void op_add();
void op_multiply();
void op_hadamard();
void op_hadamard();
void op_exp();
void op_submatrix();
void op_read();
void op_write();
