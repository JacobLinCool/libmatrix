/**
 * @file matrix.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief A generic sparse matrix implementation.
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 */

#pragma once

#include <string.h>

#include "guard.h"
#include "oxidation.h"
#include "utils.h"

#define MATRIX_STRUCT(_name, _data_type, _index_type)                                              \
	typedef struct {                                                                               \
		_index_type row;                                                                           \
		_index_type col;                                                                           \
		_data_type	val;                                                                           \
	} _name##Element;                                                                              \
                                                                                                   \
	typedef struct {                                                                               \
		bool		exists;                                                                        \
		_index_type index;                                                                         \
	} _name##Found;                                                                                \
                                                                                                   \
	typedef struct {                                                                               \
		u8				size;                                                                      \
		_name##Element* data;                                                                      \
		char*			name;                                                                      \
	} _name;

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
	void _name##_free(_name* m) {                                                                  \
		free(m->data);                                                                             \
		free(m->name);                                                                             \
		free(m);                                                                                   \
	}                                                                                              \
                                                                                                   \
	void _name##_change_name(_name* m, char* name) {                                               \
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
				return (_name##Found){true, mid};                                                  \
			} else if (m->data[mid].row < row ||                                                   \
					   (m->data[mid].row == row && m->data[mid].col < col)) {                      \
				lower = mid + 1;                                                                   \
			} else {                                                                               \
				upper = mid;                                                                       \
			}                                                                                      \
		}                                                                                          \
		return (_name##Found){false, lower};                                                       \
	}                                                                                              \
                                                                                                   \
	void _name##_set(_name* m, _index_type row, _index_type col, _data_type val) {                 \
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
				m->data = realloc(m->data, sizeof(_name##Element) * (1 << (++m->size)));           \
			}                                                                                      \
			for (_index_type i = m->data[0].val; i > found.index; i--) {                           \
				m->data[i] = m->data[i - 1];                                                       \
			}                                                                                      \
			m->data[found.index] = (_name##Element){row, col, val};                                \
			++m->data[0].val;                                                                      \
		}                                                                                          \
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
	void _name##_resize(_name* m, _index_type row, _index_type col) {                              \
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
			if (a->data[i].col != b_t->data[j].col) {                                              \
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
	}

#define MATRIX_METHOD_DECLARE(_name, _data_type, _index_type)                                      \
	_name*		 _name##_new(_index_type row, _index_type col);                                    \
	void		 _name##_free(_name* m);                                                           \
	void		 _name##_change_name(_name* m, char* name);                                        \
	_name##Found _name##_find(_name* m, _index_type row, _index_type col);                         \
	void		 _name##_set(_name* m, _index_type row, _index_type col, _data_type val);          \
	_data_type	 _name##_get(_name* m, _index_type row, _index_type col);                          \
	_data_type*	 _name##_to_1d(_name* m);                                                          \
	_data_type** _name##_to_2d(_name* m);                                                          \
	void		 _name##_resize(_name* m, _index_type row, _index_type col);                       \
	_name*		 _name##_transpose(_name* m);                                                      \
	_name*		 _name##_add(_name* a, _name* b);                                                  \
	_name*		 _name##_scale(_name* m, _data_type scalar);                                       \
	_name*		 _name##_multiply(_name* a, _name* b);                                             \
	_name*		 _name##_from_1d(_data_type* data, _index_type row, _index_type col);              \
	_name*		 _name##_from_2d(_data_type** data, _index_type row, _index_type col);

#define MATRIX(_name, _data_type, _index_type)                                                     \
	MATRIX_STRUCT(_name, _data_type, _index_type)                                                  \
	MATRIX_SAFE_GUARD(_name, _data_type, _index_type)                                              \
	MATRIX_METHOD(_name, _data_type, _index_type)

#define MATRIX_DECLARE(_name, _data_type, _index_type)                                             \
	MATRIX_STRUCT(_name, _data_type, _index_type)                                                  \
	MATRIX_SAFE_GUARD_DECLARE(_name, _data_type, _index_type)                                      \
	MATRIX_METHOD_DECLARE(_name, _data_type, _index_type)
