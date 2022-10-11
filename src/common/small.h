/**
 * @file small.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Small matrix with double data type and uint16_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define SMALL_MATRIX(_name) MATRIX(_name, double, uint16_t);
