/**
 * @file int.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Integer matrix with int32_t data type and uint32_t index type.
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define INT_MATRIX(_name) MATRIX(_name, int32_t, uint32_t);
