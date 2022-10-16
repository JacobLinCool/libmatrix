#include "operations.h"

char* ERR_NO_MATRIX = "\x1b[101m No matrices was created. \x1b[m\n";
char* ERR_NEED_2 = "\x1b[101m At least two matrices must be created. \x1b[m\n";
char* ERR_SAME_SHAPE = "\x1b[101m Matrices must have the same shape. \x1b[m\n";
char* ERR_NOT_MULTIPLYABLE = "\x1b[101m Matrices must be multiplyable. \x1b[m\n";

void op_list() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
	} else {
		for (int64_t i = 0; i < matrices.size; ++i) {
			fprintf(stderr, "    %2" PRId64 ". %s (%" PRIu64 ", %" PRIu64 ")\n", i + 1,
					matrices.matrices[i]->name, matrices.matrices[i]->data[0].row,
					matrices.matrices[i]->data[0].col);
		}
	}
}

int64_t select_matrix(char* question) {
	char** options = malloc(sizeof(char*) * matrices.size);
	for (int64_t i = 0; i < matrices.size; ++i) {
		options[i] = malloc(256 * sizeof(char));
		sprintf(options[i], "%s (%" PRIu64 ", %" PRIu64 ")", matrices.matrices[i]->name,
				matrices.matrices[i]->data[0].row, matrices.matrices[i]->data[0].col);
	}

	struct MenuController menu = MenuController_new();
	menu.question = question;
	menu.options = options;
	menu.options_len = matrices.size;

	bool normal = show_menu(&menu);

	for (int64_t i = 0; i < matrices.size; ++i) {
		free(options[i]);
	}
	free(options);

	return normal ? menu.selected : 0;
}

void op_create() {
	int64_t rows = 0;
	while (rows == 0) {
		fprintf(stderr, "Please enter the number of rows: ");
		if (scanf("%" SCNd64, &rows) != 1 || rows <= 0) {
			fprintf(stderr, "Invalid number of rows.\n");
			fflush(stdin);
			rows = 0;
		}
	}

	int64_t cols = 0;
	while (cols == 0) {
		fprintf(stderr, "Please enter the number of columns: ");
		if (scanf("%" SCNd64, &cols) != 1 || cols <= 0) {
			fprintf(stderr, "Invalid number of columns.\n");
			fflush(stdin);
			cols = 0;
		}
	}

	Matrix* m = Matrix_new(rows, cols);
	if (matrices_add(m) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(m);
		return;
	}

	fprintf(stderr, "Matrix %s created.\n", m->name);
}

void op_delete() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to delete:");
	if (index) {
		Matrix* m = matrices_remove(index - 1);
		Matrix_free(m);
		fprintf(stderr, "Matrix %s deleted.\n", m->name);
	}
}

void op_print() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to print:");
	if (index) {
		Matrix* m = matrices_get(index - 1);

		char* str = Matrix_to_string(m, -1, -1);
		fprintf(stderr, "Matrix %s:\n\n%s\n", m->name, str);
		free(str);
	}
}

void op_print_sparse() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to print:");
	if (index) {
		Matrix* m = matrices_get(index - 1);

		int max_row_width = 0;
		int max_col_width = 0;
		int max_val_width = 0;

		for (size_t i = 0; i <= m->data[0].val; i++) {
			int row_width = snprintf(NULL, 0, "%" PRIu64, m->data[i].row);
			int col_width = snprintf(NULL, 0, "%" PRIu64, m->data[i].col);
			int val_width = snprintf(NULL, 0, "%g" PRIu64, m->data[i].val);

			if (row_width > max_row_width) {
				max_row_width = row_width;
			}
			if (col_width > max_col_width) {
				max_col_width = col_width;
			}
			if (val_width > max_val_width) {
				max_val_width = val_width;
			}
		}
		int idx_width = snprintf(NULL, 0, "%g", m->data[0].val);

		fprintf(stderr, "%s\n", m->name);
		for (size_t i = 0; i <= m->data[0].val; i++) {
			fprintf(stderr, "(%*zu) %*" PRIu64 " %*" PRIu64 " %*g\n", idx_width, i, max_row_width,
					m->data[i].row, max_col_width, m->data[i].col, max_val_width, m->data[i].val);
		}
	}
}

void op_rename() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to rename:");
	if (index) {
		Matrix* m = matrices_get(index - 1);

		char name[1024] = {0};
		fprintf(stderr, "Please enter the new name of the matrix: ");
		fflush(stdin);
		while (scanf("%[^\n]%*c", name) != 1) {
			fprintf(stderr, "Invalid name.\n");
			fflush(stdin);
		}

		if (matrices_get_by_name(name)) {
			fprintf(stderr, "Failed to rename matrix, duplicate name.\n");
			return;
		}

		char old_name[1024] = {0};
		strcpy(old_name, m->name);
		Matrix_rename(m, name);
		fprintf(stderr, "Matrix \x1b[93m%s\x1b[m renamed to \x1b[96m%s\x1b[m.\n", old_name,
				m->name);
	}
}

void op_edit() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to edit:");
	if (index) {
		Matrix* m = matrices_get(index - 1);

		struct MatrixEditor editor = MatrixEditor_new();
		editor.matrix = m;
		show_matrix_editor(&editor);
	}
}

void op_transpose() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to transpose:");
	if (index) {
		Matrix* m = matrices_get(index - 1);
		Matrix* t = Matrix_transpose(m);
		if (matrices_add(t) == false) {
			fprintf(stderr, "Failed to create matrix, duplicate name.\n");
			Matrix_free(t);
			return;
		}

		char* str = Matrix_to_string(t, -1, -1);
		fprintf(stderr, "Matrix %s transposed:\n\n%s\n", m->name, str);
		free(str);
	}
}

void op_scale() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to scale:");
	if (index) {
		Matrix* m = matrices_get(index - 1);

		double scalar = 0;
		while (scalar == 0) {
			fprintf(stderr, "Please enter the scalar: ");
			if (scanf("%lf", &scalar) != 1) {
				fprintf(stderr, "Invalid scalar.\n");
				fflush(stdin);
				scalar = 0;
			}
		}

		Matrix* s = Matrix_scale(m, scalar);
		if (matrices_add(s) == false) {
			fprintf(stderr, "Failed to create matrix, duplicate name.\n");
			Matrix_free(s);
			return;
		}

		char* str = Matrix_to_string(s, -1, -1);
		fprintf(stderr, "Matrix %s scaled by %lf:\n\n%s\n", m->name, scalar, str);
		free(str);
	}
}

void op_add() {
	if (matrices.size < 2) {
		fprintf(stderr, "%s", ERR_NEED_2);
		return;
	}

	int64_t index1 = select_matrix("Select first matrix to add:");
	if (!index1) {
		return;
	}
	Matrix* m1 = matrices_get(index1 - 1);

	int64_t index2 = select_matrix("Select second matrix to add:");
	if (!index2) {
		return;
	}
	Matrix* m2 = matrices_get(index2 - 1);

	if (Matrix_shape_equal(m1, m2) == false) {
		fprintf(stderr, "%s\n", ERR_SAME_SHAPE);
		return;
	}

	Matrix* a = Matrix_add(m1, m2);
	if (matrices_add(a) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(a);
		return;
	}

	char* str = Matrix_to_string(a, -1, -1);
	fprintf(stderr, "Matrix %s + %s:\n\n%s\n", m1->name, m2->name, str);
	free(str);
}

void op_multiply() {
	if (matrices.size < 2) {
		fprintf(stderr, "%s", ERR_NEED_2);
		return;
	}

	int64_t index1 = select_matrix("Select first matrix to perform multiplication:");
	if (!index1) {
		return;
	}
	Matrix* m1 = matrices_get(index1 - 1);

	int64_t index2 = select_matrix("Select second matrix to perform multiplication:");
	if (!index2) {
		return;
	}
	Matrix* m2 = matrices_get(index2 - 1);

	if (m1->data[0].col != m2->data[0].row) {
		fprintf(stderr, "%s\n", ERR_NOT_MULTIPLYABLE);
		return;
	}

	Matrix* p = Matrix_multiply(m1, m2);
	if (matrices_add(p) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(p);
		return;
	}

	char* str = Matrix_to_string(p, -1, -1);
	fprintf(stderr, "Matrix %s * %s:\n\n%s\n", m1->name, m2->name, str);
	free(str);
}

void op_hadamard() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index1 = select_matrix("Select first matrix to perform element-wise multiplication:");
	if (!index1) {
		return;
	}
	Matrix* m1 = matrices_get(index1 - 1);

	int64_t index2 = select_matrix("Select second matrix to perform element-wise multiplication:");
	if (!index2) {
		return;
	}
	Matrix* m2 = matrices_get(index2 - 1);

	if (Matrix_shape_equal(m1, m2) == false) {
		fprintf(stderr, "%s\n", ERR_SAME_SHAPE);
		return;
	}

	Matrix* p = Matrix_hadamard(m1, m2);
	if (matrices_add(p) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(p);
		return;
	}

	char* str = Matrix_to_string(p, -1, -1);
	fprintf(stderr, "Matrix %s * %s (element-wise):\n\n%s\n", m1->name, m2->name, str);
	free(str);
}

void op_exp() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to perform exponentiate:");
	if (!index) {
		return;
	}
	Matrix* m = matrices_get(index - 1);

	int64_t exp = -1;
	while (exp == -1) {
		fprintf(stderr, "Please enter the exponent (non-negative integer): ");
		if (scanf("%" SCNd64, &exp) != 1) {
			fprintf(stderr, "Invalid exponent.\n");
			fflush(stdin);
			exp = -1;
		}
	}

	Matrix* result = Matrix_exp(m, exp);
	if (matrices_add(result) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(result);
		return;
	}

	char* str = Matrix_to_string(result, -1, -1);
	fprintf(stderr, "Matrix %s ^ %" PRId64 ":\n\n%s\n", m->name, exp, str);
	free(str);
}

void op_submatrix() {
	if (matrices.size == 0) {
		fprintf(stderr, "%s", ERR_NO_MATRIX);
		return;
	}

	int64_t index = select_matrix("Select a matrix to perform exponentiate:");
	if (!index) {
		return;
	}
	Matrix* m = matrices_get(index - 1);

	struct MatrixRowColSelector selector = MatrixRowColSelector_new();
	selector.matrix = m;
	selector.rows = calloc(m->data[0].row, sizeof(bool));
	selector.cols = calloc(m->data[0].col, sizeof(bool));

	bool done = show_matrix_row_col_selector(&selector);

	if (done == false) {
		free(selector.rows);
		free(selector.cols);
		return;
	}

	bool has_row = false, has_col = false;
	for (uint64_t i = 0; i < m->data[0].row; i++) {
		if (selector.rows[i]) {
			has_row = true;
			break;
		}
	}
	for (uint64_t i = 0; i < m->data[0].col; i++) {
		if (selector.cols[i]) {
			has_col = true;
			break;
		}
	}

	if (has_row == false || has_col == false) {
		fprintf(stderr, "Unable to create submatrix, no row or column selected.\n");
		free(selector.rows);
		free(selector.cols);
		return;
	}

	Matrix* sub = Matrix_submatrix(m, selector.rows, selector.cols);

	free(selector.rows);
	free(selector.cols);

	if (matrices_add(sub) == false) {
		fprintf(stderr, "Failed to create matrix, duplicate name.\n");
		Matrix_free(sub);
		return;
	}

	char* str = Matrix_to_string(sub, -1, -1);
	fprintf(stderr, "Submatrix of %s:\n\n%s\n", m->name, str);
	free(str);
}

FILE* resolve_matrices_file(const char* filename, const char* mode) {
	size_t len = strlen(filename);
	char*  exts[] = {".matrices", ".matrix", ".txt", ""};

	for (size_t i = 0; i < sizeof(exts) / sizeof(char*); ++i) {
		size_t path_len = len + strlen(exts[i]);
		char   path[path_len + 1];
		memset(path, 0, path_len + 1);
		strcat(path, filename);
		strcat(path, exts[i]);

		FILE* file = fopen(path, mode);
		if (file) {
			return file;
		}
	}

	return NULL;
}

void op_read() {
	char filename[1024];
	fprintf(stderr, "Please enter the filename that contains matrices: ");
	scanf("%1023[^\n]", filename);
	fflush(stdin);

	FILE* file = resolve_matrices_file(filename, "r");
	if (!file) {
		fprintf(stderr, "\x1b[101m Failed to open file \x1b[93m%s\x1b[39m. \x1b[m\n", filename);
		return;
	}

	char	name[1024] = {0};
	int64_t row, col;
	while (fscanf(file, "%[^\n]", name) == 1 &&
		   fscanf(file, "%" SCNd64 " %" SCNd64, &row, &col) == 2) {
		if (strlen(name) < 1 || row < 1 || col < 1) {
			fprintf(stderr, "\x1b[101m Invalid matrix name or shape. \x1b[m\n");
			continue;
		}

		Matrix* m = Matrix_new(row, col);
		Matrix_rename(m, name);

		bool failed = false;
		for (int64_t i = 0; i < row; ++i) {
			for (int64_t j = 0; j < col; ++j) {
				double val;
				if (fscanf(file, "%lf", &val) != 1) {
					failed = true;
					break;
				}

				Matrix_set(m, i, j, val);
			}

			if (failed) {
				break;
			}
		}

		if (failed) {
			fprintf(stderr, "\x1b[101m Failed to read matrix data. \x1b[m\n");
			Matrix_free(m);
			return;
		}

		if (matrices_add(m) == false) {
			fprintf(stderr,
					"\x1b[91mFailed to load matrix \x1b[93m%s\x1b[91m, duplicate name.\x1b[m\n",
					m->name);
			Matrix_free(m);
		} else {
			fprintf(stderr, "\x1b[92mMatrix \x1b[93m%s\x1b[92m has been loaded.\x1b[m\n", m->name);
		}

		memset(name, 0, sizeof(name));
		row = col = 0;

		int bytes;
		while (fscanf(file, "\n%n", &bytes) == 1) {
			if (bytes != 1) {
				break;
			}
		}
	}

	fclose(file);
}

void op_write() {
	char filename[1024];
	fprintf(stderr, "Please enter the filename to save matrices: ");
	scanf("%1023s", filename);
	fflush(stdin);

	FILE* file = resolve_matrices_file(filename, "w");
	if (!file) {
		fprintf(stderr, "\x1b[101m Failed to open file \x1b[93m%s\x1b[39m. \x1b[m\n", filename);
		return;
	}

	for (int64_t i = 0; i < matrices.size; i++) {
		Matrix* m = matrices_get(i);
		fprintf(file, "%s\n%" PRId64 " %" PRId64 "\n", m->name, m->data[0].row, m->data[0].col);

		int max_width = 0;
		for (uint32_t i = 0; i < m->data[0].row; ++i) {
			for (uint32_t j = 0; j < m->data[0].col; ++j) {
				int width = snprintf(NULL, 0, "%.9lg", Matrix_get(m, i, j));
				if (width > max_width) {
					max_width = width;
				}
			}
		}

		for (uint32_t i = 0; i < m->data[0].row; ++i) {
			for (uint32_t j = 0; j < m->data[0].col; ++j) {
				double val = Matrix_get(m, i, j);
				fprintf(file, "%*.9lg ", max_width, val);
			}
			fprintf(file, "\n");
		}
		fprintf(file, "\n");

		fprintf(stderr, "\x1b[92mMatrix \x1b[93m%s\x1b[92m has been saved. \x1b[m\n", m->name);
	}

	fclose(file);
}
