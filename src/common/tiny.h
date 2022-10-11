/**
 * @file tiny.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Tiny matrix with float data type and uint8_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define TINY_MATRIX(_name) MATRIX(_name, float, uint8_t);
