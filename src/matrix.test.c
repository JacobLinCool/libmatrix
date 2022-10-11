#include "matrix.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MATRIX_STRUCT(Matrix, f64, u32);
MATRIX(Matrix, f64, u32);

void test_operations();

int main() {
	srand(1481);

	Matrix* matrix = Matrix_new(2, 2);
	Matrix_rename(matrix, "matrix");
	assert(strcmp(matrix->name, "matrix") == 0);
	assert(matrix->data[0].row == 2);
	assert(matrix->data[0].col == 2);
	assert(matrix->data[0].val == 0);
	assert(matrix->size == 1);

	MatrixFound result_before = Matrix_find(matrix, 2, 2);
	assert(result_before.exists == false);
	assert(result_before.index == 1);

	Matrix_set(matrix, 0, 0, 1.0);
	assert(matrix->size == 1);
	Matrix_set(matrix, 0, 1, 2.0);
	assert(matrix->size == 2);
	Matrix_set(matrix, 1, 0, 3.0);
	assert(matrix->size == 2);
	Matrix_set(matrix, 1, 1, 4.0);
	assert(matrix->size == 3);

	MatrixFound result_after = Matrix_find(matrix, 2, 2);
	assert(result_after.exists == false);
	assert(result_after.index == 5);

	f64* matrix_1d = Matrix_to_1d(matrix);
	for (u32 i = 0; i < 4; ++i) {
		assert(matrix_1d[i] == i + 1);
	}
	free(matrix_1d);

	f64** matrix_2d = Matrix_to_2d(matrix);
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 2; ++j) {
			assert(matrix_2d[i][j] == i * 2 + j + 1);
		}
		free(matrix_2d[i]);
	}
	free(matrix_2d);

	assert(Matrix_get(matrix, 0, 0) == 1.0);
	assert(Matrix_get(matrix, 0, 1) == 2.0);
	assert(Matrix_get(matrix, 1, 0) == 3.0);
	assert(Matrix_get(matrix, 1, 1) == 4.0);

	Matrix* transposed = Matrix_transpose(matrix);
	assert(transposed->size == 3);
	assert(transposed->data[0].row == 2);
	assert(transposed->data[0].col == 2);
	assert(transposed->data[0].val == 4);

	assert(Matrix_get(transposed, 0, 0) == 1.0);
	assert(Matrix_get(transposed, 0, 1) == 3.0);
	assert(Matrix_get(transposed, 1, 0) == 2.0);
	assert(Matrix_get(transposed, 1, 1) == 4.0);

	Matrix_free(transposed);
	Matrix_free(matrix);

	Matrix* id = Matrix_identity(3);
	assert(id->data[0].row == 3);
	assert(id->data[0].col == 3);
	assert(id->data[0].val == 3);

	assert(Matrix_get(id, 0, 0) == 1.0);
	assert(Matrix_get(id, 0, 1) == 0.0);
	assert(Matrix_get(id, 0, 2) == 0.0);
	assert(Matrix_get(id, 1, 0) == 0.0);
	assert(Matrix_get(id, 1, 1) == 1.0);
	assert(Matrix_get(id, 1, 2) == 0.0);
	assert(Matrix_get(id, 2, 0) == 0.0);
	assert(Matrix_get(id, 2, 1) == 0.0);
	assert(Matrix_get(id, 2, 2) == 1.0);
	Matrix_free(id);

	test_operations();

	Matrix* invalid = Matrix_new(3, 3);
	invalid->data[0].val = 5;
	assert(Matrix_validate(invalid) == false);
	Matrix_rebuild(invalid);
	assert(invalid->size == 3);
	invalid->data[1].row = invalid->data[1].col = invalid->data[1].val = 2;
	invalid->data[2].row = invalid->data[2].col = invalid->data[2].val = 1;
	invalid->data[3].row = invalid->data[3].col = invalid->data[3].val = 0;
	invalid->data[4].row = 2;
	invalid->data[4].col = 0;
	invalid->data[4].val = -1;
	invalid->data[5].row = 0;
	invalid->data[5].col = 2;
	invalid->data[5].val = -2;
	assert(Matrix_validate(invalid) == false);
	Matrix_rebuild(invalid);
	assert(Matrix_validate(invalid) == true);
	assert(invalid->data[1].row == 0 && invalid->data[1].col == 0 && invalid->data[1].val == 0);
	assert(invalid->data[2].row == 0 && invalid->data[2].col == 2 && invalid->data[2].val == -2);
	assert(invalid->data[3].row == 1 && invalid->data[3].col == 1 && invalid->data[3].val == 1);
	assert(invalid->data[4].row == 2 && invalid->data[4].col == 0 && invalid->data[4].val == -1);
	assert(invalid->data[5].row == 2 && invalid->data[5].col == 2 && invalid->data[5].val == 2);

	return EXIT_SUCCESS;
}

void test_operations() {
	Matrix* a = Matrix_from_1d((f64[]){1.0, 2.0, 3.0, 4.0}, 2, 2);
	Matrix* b = Matrix_from_1d((f64[]){5.0, 6.0, 7.0, 8.0}, 2, 2);

	Matrix* c = Matrix_add(a, b);
	assert(c->data[0].row == 2);
	assert(c->data[0].col == 2);
	assert(c->data[0].val == 4);

	assert(Matrix_get(c, 0, 0) == 6.0);
	assert(Matrix_get(c, 0, 1) == 8.0);
	assert(Matrix_get(c, 1, 0) == 10.0);
	assert(Matrix_get(c, 1, 1) == 12.0);
	Matrix_free(c);

	Matrix* d = Matrix_scale(a, 2.0);
	assert(d->data[0].row == 2);
	assert(d->data[0].col == 2);
	assert(d->data[0].val == 4);

	assert(Matrix_get(d, 0, 0) == 2.0);
	assert(Matrix_get(d, 0, 1) == 4.0);
	assert(Matrix_get(d, 1, 0) == 6.0);
	assert(Matrix_get(d, 1, 1) == 8.0);
	Matrix_free(d);

	Matrix* e = Matrix_multiply(a, b);
	assert(e->data[0].row == 2);
	assert(e->data[0].col == 2);
	assert(e->data[0].val == 4);

	assert(Matrix_get(e, 0, 0) == 19.0);
	assert(Matrix_get(e, 0, 1) == 22.0);
	assert(Matrix_get(e, 1, 0) == 43.0);
	assert(Matrix_get(e, 1, 1) == 50.0);
	Matrix_free(e);

	Matrix* f = Matrix_hadamard(a, b);
	assert(f->data[0].row == 2);
	assert(f->data[0].col == 2);
	assert(f->data[0].val == 4);

	assert(Matrix_get(f, 0, 0) == 5.0);
	assert(Matrix_get(f, 0, 1) == 12.0);
	assert(Matrix_get(f, 1, 0) == 21.0);
	assert(Matrix_get(f, 1, 1) == 32.0);
	Matrix_free(f);

	Matrix* g = Matrix_exp(a, 11);
	assert(g->data[0].row == 2);
	assert(g->data[0].col == 2);
	assert(g->data[0].val == 4);

	assert(Matrix_get(g, 0, 0) == 25699957.0);
	assert(Matrix_get(g, 0, 1) == 37455814.0);
	assert(Matrix_get(g, 1, 0) == 56183721.0);
	assert(Matrix_get(g, 1, 1) == 81883678.0);
	Matrix_free(g);

	Matrix* matrix = Matrix_from_1d(
		(f64[]){1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0}, 3, 4);

	Matrix* sub = Matrix_submatrix(matrix, (bool[]){1, 0, 1}, (bool[]){0, 1, 1, 0});
	assert(sub->data[0].row == 2);
	assert(sub->data[0].col == 2);
	assert(sub->data[0].val == 4);

	assert(Matrix_get(sub, 0, 0) == 2.0);
	assert(Matrix_get(sub, 0, 1) == 3.0);
	assert(Matrix_get(sub, 1, 0) == 10.0);
	assert(Matrix_get(sub, 1, 1) == 11.0);
	Matrix_free(sub);
	Matrix_free(matrix);
}
