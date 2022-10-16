#include "matrices.h"

void matrices_init() {
	matrices.size = 0;
	matrices.capacity = 1;
	matrices.matrices = malloc(sizeof(Matrix*) * matrices.capacity);
}

void matrices_free() { free(matrices.matrices); }

bool matrices_add(Matrix* m) {
	for (u32 i = 0; i < matrices.size; ++i) {
		if (strcmp(matrices.matrices[i]->name, m->name) == 0) {
			return false;
		}
	}

	matrices.size++;
	if (matrices.size > matrices.capacity) {
		matrices.capacity <<= 1;
		matrices.matrices = realloc(matrices.matrices, sizeof(Matrix*) * matrices.capacity);
	}
	matrices.matrices[matrices.size - 1] = m;

	return true;
}

int64_t matrices_find(char* name) {
	for (int64_t i = 0; i < matrices.size; ++i) {
		if (strcmp(matrices.matrices[i]->name, name) == 0) {
			return i;
		}
	}

	return -1;
}

Matrix* matrices_remove(int64_t index) {
	Matrix* m = matrices.matrices[index];
	matrices.size--;
	for (int64_t i = index; i < matrices.size; ++i) {
		matrices.matrices[i] = matrices.matrices[i + 1];
	}
	matrices.matrices[matrices.size] = NULL;
	return m;
}

Matrix* matrices_get(int64_t index) {
	if (index >= matrices.size) {
		return NULL;
	}
	return matrices.matrices[index];
}

Matrix* matrices_get_by_name(char* name) {
	int64_t index = matrices_find(name);
	if (index == -1) {
		return NULL;
	}
	return matrices.matrices[index];
}
