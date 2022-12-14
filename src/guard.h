/**
 * @file guard.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Safe guard for matrix.h
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include "oxidation.h"

#ifdef SAFE_MATRIX

#define MATRIX_SAFE_GUARD(_matrix_type, _data_type, _index_type)                                   \
	static inline bool _matrix_type##_out_range(_matrix_type* m, _index_type row,                  \
												_index_type col) {                                 \
		return row < 0 || row >= m->data[0].row || col < 0 || col >= m->data[0].col;               \
	}

#else

#define MATRIX_SAFE_GUARD(_matrix_type, _data_type, _index_type)                                   \
	static inline bool _matrix_type##_out_range(_matrix_type* m, _index_type row,                  \
												_index_type col) {                                 \
		return false;                                                                              \
	}

#endif

#define MATRIX_SAFE_GUARD_DECLARE(_matrix_type, _data_type, _index_type)                           \
	static inline bool _matrix_type##_out_range(_matrix_type* m, _index_type row, _index_type col);
