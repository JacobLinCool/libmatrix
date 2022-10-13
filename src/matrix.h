/**
 * @file matrix.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief A generic sparse matrix implementation.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 */

#pragma once

#include <string.h>

#include "guard.h"
#include "oxidation.h"
#include "utils.h"

#ifdef DEBUG
#define PRINT(...) printf(__VA_ARGS__)
#else
#define PRINT(...)
#endif

#define MATRIX_STRUCT(_name, _data_type, _index_type)                                              \
	typedef struct _name##Element {                                                                \
		_index_type row;                                                                           \
		_index_type col;                                                                           \
		_data_type	val;                                                                           \
	} _name##Element;                                                                              \
                                                                                                   \
	typedef struct _name##Found {                                                                  \
		bool		exists;                                                                        \
		_index_type index;                                                                         \
	} _name##Found;                                                                                \
                                                                                                   \
	typedef struct _name {                                                                         \
		u8				size;                                                                      \
		_name##Element* data;                                                                      \
		char*			name;                                                                      \
	} _name;

#define MATRIX_STRUCT_DECLARE(_name, _data_type, _index_type)                                      \
	typedef struct _name##Element _name##Element;                                                  \
	typedef struct _name##Found	  _name##Found;                                                    \
	typedef struct _name		  _name;

#define MATRIX_METHOD(_name, _data_type, _index_type)                                              \
	_name* _name##_new(_index_type row, _index_type col) {                                         \
		_name* m = malloc(sizeof(_name));                                                          \
		m->size = 1;                                                                               \
		m->data = malloc(sizeof(_name##Element) * (1 << 1));                                       \
		m->data[0] = (_name##Element){row, col, 0};                                                \
		m->name = random_name(4);                                                                  \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_identity(_index_type size) {                                                    \
		_name* m = malloc(sizeof(_name));                                                          \
		u64	   init_size = 1, tmp = size;                                                          \
		while (tmp >>= 1) {                                                                        \
			++init_size;                                                                           \
		}                                                                                          \
		m->size = init_size;                                                                       \
		m->data = malloc(sizeof(_name##Element) * (1 << init_size));                               \
		m->data[0] = (_name##Element){size, size, size};                                           \
		for (u64 i = 0; i < size; ++i) {                                                           \
			m->data[i + 1] = (_name##Element){i, i, 1};                                            \
		}                                                                                          \
		m->name = random_name(4);                                                                  \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	void _name##_free(_name* m) {                                                                  \
		free(m->data);                                                                             \
		free(m->name);                                                                             \
		free(m);                                                                                   \
	}                                                                                              \
                                                                                                   \
	void _name##_rename(_name* m, char* name) {                                                    \
		free(m->name);                                                                             \
		m->name = strdup(name);                                                                    \
	}                                                                                              \
                                                                                                   \
	_name##Found _name##_find(_name* m, _index_type row, _index_type col) {                        \
		if (_name##_out_range(m, row, col)) {                                                      \
			return (_name##Found){false, 0};                                                       \
		}                                                                                          \
		_index_type lower = 1;                                                                     \
		_index_type upper = m->data[0].val + 1;                                                    \
		while (lower < upper) {                                                                    \
			_index_type mid = (lower + upper) / 2;                                                 \
			if (m->data[mid].row == row && m->data[mid].col == col) {                              \
				PRINT("Found at %d\n", mid);                                                       \
				return (_name##Found){true, mid};                                                  \
			} else if (m->data[mid].row < row ||                                                   \
					   (m->data[mid].row == row && m->data[mid].col < col)) {                      \
				lower = mid + 1;                                                                   \
			} else {                                                                               \
				upper = mid;                                                                       \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		PRINT("Not found, fall to %d\n", lower);                                                   \
		return (_name##Found){false, lower};                                                       \
	}                                                                                              \
                                                                                                   \
	void _name##_set(_name* m, _index_type row, _index_type col, _data_type val) {                 \
		PRINT("\x1b[93m" #_name "_set %d %d %d start\x1b[m\n", row, col, val);                     \
		if (_name##_out_range(m, row, col)) {                                                      \
			return;                                                                                \
		}                                                                                          \
		_name##Found found = _name##_find(m, row, col);                                            \
		if (val == 0) {                                                                            \
			if (found.exists) {                                                                    \
				for (_index_type i = found.index; i < m->data[0].val; ++i) {                       \
					m->data[i] = m->data[i + 1];                                                   \
				}                                                                                  \
				memset(m->data + (size_t)m->data[0].val, 0, sizeof(_name##Element));               \
				m->data[0].val--;                                                                  \
			}                                                                                      \
			return;                                                                                \
		}                                                                                          \
		if (found.exists) {                                                                        \
			m->data[found.index].val = val;                                                        \
		} else {                                                                                   \
			if ((1 << m->size) <= m->data[0].val + 1) {                                            \
				PRINT("Reallocating to %d\n", 1 << (m->size + 1));                                 \
				m->data = realloc(m->data, sizeof(_name##Element) * (1 << ++m->size));             \
			}                                                                                      \
			for (_index_type i = m->data[0].val; i >= found.index; --i) {                          \
				PRINT("Moving %d to %d\n", i, i + 1);                                              \
				memcpy(m->data + i + 1, m->data + i, sizeof(_name##Element));                      \
			}                                                                                      \
			m->data[found.index] = (_name##Element){row, col, val};                                \
			++m->data[0].val;                                                                      \
		}                                                                                          \
		PRINT(#_name "_set %d %d %d end\n", row, col, val);                                        \
	}                                                                                              \
                                                                                                   \
	_data_type _name##_get(_name* m, _index_type row, _index_type col) {                           \
		if (_name##_out_range(m, row, col)) {                                                      \
			return 0;                                                                              \
		}                                                                                          \
		_name##Found found = _name##_find(m, row, col);                                            \
		if (found.exists) {                                                                        \
			return m->data[found.index].val;                                                       \
		} else {                                                                                   \
			return 0;                                                                              \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	_data_type* _name##_to_1d(_name* m) {                                                          \
		_data_type* arr = malloc(sizeof(_data_type) * m->data[0].row * m->data[0].col);            \
		for (_index_type i = 0; i < m->data[0].row * m->data[0].col; ++i) {                        \
			arr[i] = 0;                                                                            \
		}                                                                                          \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			arr[m->data[i].row * m->data[0].col + m->data[i].col] = m->data[i].val;                \
		}                                                                                          \
		return arr;                                                                                \
	}                                                                                              \
                                                                                                   \
	_data_type** _name##_to_2d(_name* m) {                                                         \
		_data_type** arr = malloc(sizeof(_data_type*) * m->data[0].row);                           \
		for (_index_type i = 0; i < m->data[0].row; ++i) {                                         \
			arr[i] = calloc(m->data[0].col, sizeof(_data_type));                                   \
		}                                                                                          \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			arr[m->data[i].row][m->data[i].col] = m->data[i].val;                                  \
		}                                                                                          \
		return arr;                                                                                \
	}                                                                                              \
                                                                                                   \
	void _name##_reshape(_name* m, _index_type row, _index_type col) {                             \
		m->data[0].row = row;                                                                      \
		m->data[0].col = col;                                                                      \
                                                                                                   \
		for (_index_type i = m->data[0].val; i > 0; --i) {                                         \
			if (m->data[i].row >= row || m->data[i].col >= col) {                                  \
				memset(m->data + i, 0, sizeof(_name##Element));                                    \
				--m->data[0].val;                                                                  \
			}                                                                                      \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	_name* _name##_transpose(_name* m) {                                                           \
		_name* t = _name##_new(m->data[0].col, m->data[0].row);                                    \
		while ((1 << t->size) <= m->data[0].val + 1) {                                             \
			t->data = realloc(t->data, sizeof(_name##Element) * (1 << (++t->size)));               \
		}                                                                                          \
                                                                                                   \
		_index_type row_terms[m->data[0].col];                                                     \
		_index_type starting_pos[m->data[0].col];                                                  \
		memset(row_terms, 0, sizeof(_index_type) * (size_t)m->data[0].col);                        \
		memset(starting_pos, 0, sizeof(_index_type) * (size_t)m->data[0].col);                     \
                                                                                                   \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			++row_terms[m->data[i].col];                                                           \
		}                                                                                          \
                                                                                                   \
		starting_pos[0] = 1;                                                                       \
		for (_index_type i = 1; i < m->data[0].col; ++i) {                                         \
			starting_pos[i] = starting_pos[i - 1] + row_terms[i - 1];                              \
		}                                                                                          \
                                                                                                   \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			_index_type idx = starting_pos[m->data[i].col]++;                                      \
			t->data[idx].row = m->data[i].col;                                                     \
			t->data[idx].col = m->data[i].row;                                                     \
			t->data[idx].val = m->data[i].val;                                                     \
		}                                                                                          \
		t->data[0].val = m->data[0].val;                                                           \
                                                                                                   \
		return t;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_add(_name* a, _name* b) {                                                       \
		_name*		m = _name##_new(a->data[0].row, a->data[0].col);                               \
		_index_type i = 1, j = 1;                                                                  \
		while (i <= a->data[0].val && j <= b->data[0].val) {                                       \
			if (a->data[i].row < b->data[j].row) {                                                 \
				_name##_set(m, a->data[i].row, a->data[i].col, a->data[i].val);                    \
				++i;                                                                               \
			} else if (a->data[i].row > b->data[j].row) {                                          \
				_name##_set(m, b->data[j].row, b->data[j].col, b->data[j].val);                    \
				++j;                                                                               \
			} else {                                                                               \
				if (a->data[i].col < b->data[j].col) {                                             \
					_name##_set(m, a->data[i].row, a->data[i].col, a->data[i].val);                \
					++i;                                                                           \
				} else if (a->data[i].col > b->data[j].col) {                                      \
					_name##_set(m, b->data[j].row, b->data[j].col, b->data[j].val);                \
					++j;                                                                           \
				} else {                                                                           \
					_name##_set(m, a->data[i].row, a->data[i].col,                                 \
								a->data[i].val + b->data[j].val);                                  \
					++i, ++j;                                                                      \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		while (i <= a->data[0].val) {                                                              \
			_name##_set(m, a->data[i].row, a->data[i].col, a->data[i].val);                        \
			++i;                                                                                   \
		}                                                                                          \
                                                                                                   \
		while (j <= b->data[0].val) {                                                              \
			_name##_set(m, b->data[j].row, b->data[j].col, b->data[j].val);                        \
			++j;                                                                                   \
		}                                                                                          \
                                                                                                   \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_scale(_name* m, _data_type scalar) {                                            \
		_name* n = _name##_new(m->data[0].row, m->data[0].col);                                    \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			_name##_set(n, m->data[i].row, m->data[i].col, scalar * m->data[i].val);               \
		}                                                                                          \
		return n;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_multiply(_name* a, _name* b) {                                                  \
		_name* m = _name##_new(a->data[0].row, b->data[0].col);                                    \
		_name* b_t = _name##_transpose(b);                                                         \
                                                                                                   \
		_index_type i = 1, j = 1;                                                                  \
		_index_type a_row_start = 1, b_row_start = 1;                                              \
		_data_type	sum = 0;                                                                       \
                                                                                                   \
		while (i <= a->data[0].val) {                                                              \
			if (a->data[i].col == b_t->data[j].col) {                                              \
				sum += a->data[i].val * b_t->data[j].val;                                          \
				++j;                                                                               \
			} else if (a->data[i].col < b_t->data[j].col) {                                        \
				++i;                                                                               \
			} else if (a->data[i].col > b_t->data[j].col) {                                        \
				++j;                                                                               \
			}                                                                                      \
                                                                                                   \
			if (i <= a->data[0].val && a->data[i].row != a->data[a_row_start].row) {               \
				i = a_row_start;                                                                   \
				++j;                                                                               \
			}                                                                                      \
                                                                                                   \
			if (j <= b_t->data[0].val && b_t->data[j].row != b_t->data[b_row_start].row) {         \
				if (sum != 0) {                                                                    \
					_name##_set(m, a->data[a_row_start].row, b_t->data[b_row_start].row, sum);     \
					sum = 0;                                                                       \
				}                                                                                  \
				b_row_start = j;                                                                   \
				i = a_row_start;                                                                   \
			}                                                                                      \
                                                                                                   \
			if (j > b_t->data[0].val) {                                                            \
				if (sum != 0) {                                                                    \
					_name##_set(m, a->data[a_row_start].row, b_t->data[b_row_start].row, sum);     \
					sum = 0;                                                                       \
				}                                                                                  \
				while (i <= a->data[0].val && a->data[i].row == a->data[a_row_start].row) {        \
					++i;                                                                           \
				}                                                                                  \
				a_row_start = i;                                                                   \
				j = b_row_start = 1;                                                               \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		_name##_free(b_t);                                                                         \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_hadamard(_name* a, _name* b) {                                                  \
		_name* m = _name##_new(a->data[0].row, a->data[0].col);                                    \
                                                                                                   \
		_index_type i = 1, j = 1;                                                                  \
		while (i <= a->data[0].val && j <= b->data[0].val) {                                       \
			if (a->data[i].row < b->data[j].row) {                                                 \
				++i;                                                                               \
			} else if (a->data[i].row > b->data[j].row) {                                          \
				++j;                                                                               \
			} else if (a->data[i].col < b->data[j].col) {                                          \
				++i;                                                                               \
			} else if (a->data[i].col > b->data[j].col) {                                          \
				++j;                                                                               \
			} else {                                                                               \
				_name##_set(m, a->data[i].row, a->data[i].col, a->data[i].val * b->data[j].val);   \
				++i, ++j;                                                                          \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_from_1d(_data_type* data, _index_type row, _index_type col) {                   \
		_name* m = _name##_new(row, col);                                                          \
		for (_index_type i = 0; i < row; ++i) {                                                    \
			for (_index_type j = 0; j < col; ++j) {                                                \
				if (data[i * col + j] != 0) {                                                      \
					_name##_set(m, i, j, data[i * col + j]);                                       \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_from_2d(_data_type** data, _index_type row, _index_type col) {                  \
		_name* m = _name##_new(row, col);                                                          \
		for (_index_type i = 0; i < row; ++i) {                                                    \
			for (_index_type j = 0; j < col; ++j) {                                                \
				if (data[i][j] != 0) {                                                             \
					_name##_set(m, i, j, data[i][j]);                                              \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
		return m;                                                                                  \
	}                                                                                              \
                                                                                                   \
	_name* _name##_submatrix(_name* m, bool* rows, bool* cols) {                                   \
		_index_type row_map[m->data[0].row], col_map[m->data[0].col];                              \
		row_map[0] = rows[0] ? 1 : 0;                                                              \
		col_map[0] = cols[0] ? 1 : 0;                                                              \
                                                                                                   \
		for (_index_type i = 1; i < m->data[0].row; ++i) {                                         \
			row_map[i] = rows[i] ? row_map[i - 1] + 1 : row_map[i - 1];                            \
		}                                                                                          \
		for (_index_type i = 1; i < m->data[0].col; ++i) {                                         \
			col_map[i] = cols[i] ? col_map[i - 1] + 1 : col_map[i - 1];                            \
		}                                                                                          \
                                                                                                   \
		_name* sub = _name##_new(row_map[m->data[0].row - 1], col_map[m->data[0].col - 1]);        \
                                                                                                   \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			if (rows[m->data[i].row] && cols[m->data[i].col]) {                                    \
				_name##_set(sub, row_map[m->data[i].row] - 1, col_map[m->data[i].col] - 1,         \
							m->data[i].val);                                                       \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		return sub;                                                                                \
	}                                                                                              \
                                                                                                   \
	_name* _name##_exp(_name* m, i64 exp) {                                                        \
		_name* base = _name##_scale(m, 1);                                                         \
		_name* ans = _name##_identity(m->data[0].row);                                             \
                                                                                                   \
		while (exp > 0) {                                                                          \
			if (exp % 2 == 1) {                                                                    \
				_name* tmp = _name##_multiply(ans, base);                                          \
				_name##_free(ans);                                                                 \
				ans = tmp;                                                                         \
			}                                                                                      \
			_name* tmp = _name##_multiply(base, base);                                             \
			_name##_free(base);                                                                    \
			base = tmp;                                                                            \
			exp >>= 1;                                                                             \
		}                                                                                          \
                                                                                                   \
		_name##_free(base);                                                                        \
		return ans;                                                                                \
	}                                                                                              \
                                                                                                   \
	bool _name##_validate(_name* m) {                                                              \
		if (m->data[0].row <= 0 || m->data[0].col <= 0) {                                          \
			return false;                                                                          \
		}                                                                                          \
                                                                                                   \
		_index_type capacity = 1;                                                                  \
		for (_index_type i = 1; i <= m->size; ++i) {                                               \
			capacity <<= 1;                                                                        \
		}                                                                                          \
		--capacity;                                                                                \
		if (m->data[0].val > capacity) {                                                           \
			return false;                                                                          \
		}                                                                                          \
                                                                                                   \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			if (m->data[i].row >= m->data[0].row || m->data[i].col >= m->data[0].col) {            \
				return false;                                                                      \
			}                                                                                      \
                                                                                                   \
			if (i > 1 && m->data[i].row < m->data[i - 1].row) {                                    \
				return false;                                                                      \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		return true;                                                                               \
	}                                                                                              \
                                                                                                   \
	int _name##Element_compare(const void* a, const void* b) {                                     \
		_name##Element* x = (_name##Element*)a;                                                    \
		_name##Element* y = (_name##Element*)b;                                                    \
                                                                                                   \
		if (x->row < y->row) {                                                                     \
			return -1;                                                                             \
		} else if (x->row > y->row) {                                                              \
			return 1;                                                                              \
		} else {                                                                                   \
			if (x->col < y->col) {                                                                 \
				return -1;                                                                         \
			} else if (x->col > y->col) {                                                          \
				return 1;                                                                          \
			} else {                                                                               \
				return 0;                                                                          \
			}                                                                                      \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	void _name##_rebuild(_name* m) {                                                               \
		u8 size = m->size;                                                                         \
                                                                                                   \
		while (m->data[0].val > (1 << size) - 1) {                                                 \
			++size;                                                                                \
		}                                                                                          \
		m->data = realloc(m->data, sizeof(_name##Element) * (1 << size));                          \
		m->size = size;                                                                            \
                                                                                                   \
		qsort(m->data + 1, m->data[0].val, sizeof(_name##Element), _name##Element_compare);        \
	}                                                                                              \
                                                                                                   \
	bool _name##_shape_equal(_name* a, _name* b) {                                                 \
		if (a->data[0].row != b->data[0].row || a->data[0].col != b->data[0].col) {                \
			return false;                                                                          \
		}                                                                                          \
		return true;                                                                               \
	}                                                                                              \
                                                                                                   \
	bool _name##_equal(_name* a, _name* b) {                                                       \
		if (a->data[0].row != b->data[0].row || a->data[0].col != b->data[0].col ||                \
			a->data[0].val != b->data[0].val) {                                                    \
			return false;                                                                          \
		}                                                                                          \
                                                                                                   \
		for (_index_type i = 1; i <= a->data[0].val; ++i) {                                        \
			if (a->data[i].row != b->data[i].row || a->data[i].col != b->data[i].col ||            \
				a->data[i].val != b->data[i].val) {                                                \
				return false;                                                                      \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		return true;                                                                               \
	}                                                                                              \
                                                                                                   \
	bool _name##_is_square(_name* m) { return m->data[0].row == m->data[0].col; }                  \
                                                                                                   \
	_name* _name##_map(_name* m, _data_type (*func)(_data_type, _index_type, _index_type)) {       \
		_name* ans = _name##_new(m->data[0].row, m->data[0].col);                                  \
                                                                                                   \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			_name##_set(ans, m->data[i].row, m->data[i].col,                                       \
						func(m->data[i].val, m->data[i].row, m->data[i].col));                     \
		}                                                                                          \
                                                                                                   \
		return ans;                                                                                \
	}                                                                                              \
                                                                                                   \
	_data_type* _name##_max_value(_name* m) {                                                      \
		_data_type* ans = malloc(sizeof(_data_type));                                              \
		*ans = m->data[1].val;                                                                     \
		for (_index_type i = 2; i <= m->data[0].val; ++i) {                                        \
			if (m->data[i].val > *ans) {                                                           \
				*ans = m->data[i].val;                                                             \
			}                                                                                      \
		}                                                                                          \
		return ans;                                                                                \
	}                                                                                              \
                                                                                                   \
	_data_type* _name##_min_value(_name* m) {                                                      \
		_data_type* ans = malloc(sizeof(_data_type));                                              \
		*ans = m->data[1].val;                                                                     \
		for (_index_type i = 2; i <= m->data[0].val; ++i) {                                        \
			if (m->data[i].val < *ans) {                                                           \
				*ans = m->data[i].val;                                                             \
			}                                                                                      \
		}                                                                                          \
		return ans;                                                                                \
	}                                                                                              \
                                                                                                   \
	_data_type _name##_sum(_name* m) {                                                             \
		_data_type ans = 0;                                                                        \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			ans += m->data[i].val;                                                                 \
		}                                                                                          \
		return ans;                                                                                \
	}                                                                                              \
                                                                                                   \
	_data_type _name##_mean(_name* m) { return _name##_sum(m) / m->data[0].val; }                  \
                                                                                                   \
	_data_type _name##_trace(_name* m) {                                                           \
		_data_type ans = 0;                                                                        \
		for (_index_type i = 1; i <= m->data[0].val; ++i) {                                        \
			if (m->data[i].row == m->data[i].col) {                                                \
				ans += m->data[i].val;                                                             \
			}                                                                                      \
		}                                                                                          \
		return ans;                                                                                \
	}

#define MATRIX_METHOD_DECLARE(_name, _data_type, _index_type)                                      \
	_name*		 _name##_new(_index_type row, _index_type col);                                    \
	_name*		 _name##_identity(_index_type size);                                               \
	void		 _name##_free(_name* m);                                                           \
	void		 _name##_rename(_name* m, char* name);                                             \
	_name##Found _name##_find(_name* m, _index_type row, _index_type col);                         \
	void		 _name##_set(_name* m, _index_type row, _index_type col, _data_type val);          \
	_data_type	 _name##_get(_name* m, _index_type row, _index_type col);                          \
	_data_type*	 _name##_to_1d(_name* m);                                                          \
	_data_type** _name##_to_2d(_name* m);                                                          \
	void		 _name##_reshape(_name* m, _index_type row, _index_type col);                      \
	_name*		 _name##_transpose(_name* m);                                                      \
	_name*		 _name##_add(_name* a, _name* b);                                                  \
	_name*		 _name##_scale(_name* m, _data_type scalar);                                       \
	_name*		 _name##_multiply(_name* a, _name* b);                                             \
	_name*		 _name##_hadamard(_name* a, _name* b);                                             \
	_name*		 _name##_from_1d(_data_type* data, _index_type row, _index_type col);              \
	_name*		 _name##_from_2d(_data_type** data, _index_type row, _index_type col);             \
	_name*		 _name##_submatrix(_name* m, bool* rows, bool* cols);                              \
	_name*		 _name##_exp(_name* m, i64 exp);                                                   \
	bool		 _name##_validate(_name* m);                                                       \
	int			 _name##Element_compare(const void* a, const void* b);                             \
	void		 _name##_rebuild(_name* m);                                                        \
	bool		 _name##_shape_equal(_name* a, _name* b);                                          \
	bool		 _name##_equal(_name* a, _name* b);                                                \
	bool		 _name##_is_square(_name* m);                                                      \
	_name*		 _name##_map(_name* m, _data_type (*func)(_data_type, _index_type, _index_type));  \
	_data_type*	 _name##_max_value(_name* m);                                                      \
	_data_type*	 _name##_min_value(_name* m);                                                      \
	_data_type	 _name##_sum(_name* m);                                                            \
	_data_type	 _name##_mean(_name* m);                                                           \
	_data_type	 _name##_trace(_name* m);

/**
 * @brief You can use this macro to create a matrix type and its methods.
 */
#define MATRIX(_name, _data_type, _index_type)                                                     \
	MATRIX_SAFE_GUARD(_name, _data_type, _index_type)                                              \
	MATRIX_METHOD(_name, _data_type, _index_type)

/**
 * @brief You can use this macro to declare a matrix type and its methods in a header file.
 */
#define DECLARE_MATRIX(_name, _data_type, _index_type)                                             \
	MATRIX_STRUCT(_name, _data_type, _index_type)                                                  \
	MATRIX_SAFE_GUARD_DECLARE(_name, _data_type, _index_type)                                      \
	MATRIX_METHOD_DECLARE(_name, _data_type, _index_type)
