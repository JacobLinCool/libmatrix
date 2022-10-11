/**
 * @file short.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Short integer matrix with int16_t data type and uint16_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define SHORT_MATRIX(_name) MATRIX(_name, int16_t, uint16_t);
