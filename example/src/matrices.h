#pragma once

#include <stdint.h>
#include <string.h>

#include "matrix.h"

typedef struct Matrices {
	int64_t	 size;
	Matrix** matrices;
	int64_t	 capacity;
} Matrices;

Matrices matrices;

void	matrices_init();
void	matrices_free();
bool	matrices_add(Matrix* m);
int64_t matrices_find(char* name);
Matrix* matrices_remove(int64_t index);
Matrix* matrices_get(int64_t index);
Matrix* matrices_get_by_name(char* name);
