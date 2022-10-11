/**
 * @file huge.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Huge matrix with long double data type and uint64_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define HUGE_MATRIX(_name) MATRIX(_name, long double, uint64_t);
