/**
 * @file shortest.h
 * @author Jacob Lin (hi@jacoblin.cool)
 * @brief Shortest integer matrix with int8_t data type and uint8_t index type.
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Jacob Lin. Released under the MIT license.
 *
 */

#pragma once

#include <stdint.h>

#include "../matrix.h"

#define SHORTEST_MATRIX(_name) MATRIX(_name, int8_t, uint8_t);
