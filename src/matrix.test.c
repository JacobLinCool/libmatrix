#include "matrix.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MATRIX(Matrix, f64, u32);

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
	return EXIT_SUCCESS;
}
