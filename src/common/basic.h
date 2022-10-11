/**
 * @file basic.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Basic matrix with double data type and uint32_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define BASIC_MATRIX(_name) MATRIX(_name, double, uint32_t);
