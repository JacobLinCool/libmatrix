/**
 * @file oxidation.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Type aliases and wrapped result type.
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;
typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;
typedef float		f32;
typedef double		f64;
typedef long double f128;
typedef char*		str;

typedef struct {
	void* val;
	char* err;
} Result;

Result Ok(void* val);
Result Err(char* err);
bool   is_err(Result res);
bool   is_ok(Result res);
