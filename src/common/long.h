/**
 * @file long.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Long integer matrix with int64_t data type and uint32_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define LONG_MATRIX(_name) MATRIX(_name, int64_t, uint32_t);
