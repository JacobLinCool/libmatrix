
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "components/welcome.h"
#include "format.h"
#include "matrices.h"
#include "matrix.h"
#include "operations.h"
#include "ui.h"

int main() {
#ifdef _WIN32
	fprintf(stderr, "This program is not supported on Windows. :(\n");
	return EXIT_FAILURE;
#endif

	matrices_init();

	clear_screen();
	Component(welcome);
	take_keypress();

	char* fatal_error = NULL;
	while (fatal_error == NULL) {
		fflush(stdin);

		char* options[] = {
			"List all matrices",
			"Create a matrix",
			"Delete a matrix",
			"Print a matrix",
			"Print a matrix in sparse matrix format",
			"Rename a matrix",
			"Edit a matrix",
			"Transpose a matrix",
			"Multiply a matrix by a scalar",
			"Add two matrices",
			"Multiply two matrices",
			"Multiply two matrices (element-wise product)",
			"Expontiate a matrix",
			"Extract a submatrix from a matrix",
			"Load matrices from file",
			"Save matrices to file",
		};

		struct MenuController menu = MenuController_new();
		menu.question = "Please select an operation:";
		menu.options = options;
		menu.options_len = sizeof(options) / sizeof(options[0]);

		bool selected = show_menu(&menu);
		if (selected == false) {
			break;
		}

		switch (menu.selected) {
			case OP_LIST: {
				op_list();
				break;
			}
			case OP_CREATE: {
				op_create();
				break;
			}
			case OP_DELETE:
				op_delete();
				break;
			case OP_PRINT:
				op_print();
				break;
			case OP_PRINT_SPARSE:
				op_print_sparse();
				break;
			case OP_RENAME:
				op_rename();
				break;
			case OP_EDIT:
				op_edit();
				break;
			case OP_TRANSPOSE:
				op_transpose();
				break;
			case OP_SCALAR_MULTIPLY:
				op_scale();
				break;
			case OP_ADD:
				op_add();
				break;
			case OP_MULTIPLY:
				op_multiply();
				break;
			case OP_HADAMARD:
				op_hadamard();
				break;
			case OP_EXP:
				op_exp();
				break;
			case OP_SUBMATRIX:
				op_submatrix();
				break;
			case OP_READ:
				op_read();
				break;
			case OP_WRITE:
				op_write();
				break;
		}

		fflush(stdin);
		fprintf(stderr, "Press ans key to continue ... ");
		take_keypress();
	}

	if (fatal_error != NULL) {
		fprintf(stderr, "\x1b[31m%s\x1b[0m\n", fatal_error);
	} else {
		fprintf(stderr, "Bye!\n");
	}

	return fatal_error == NULL ? EXIT_SUCCESS : EXIT_FAILURE;
}
