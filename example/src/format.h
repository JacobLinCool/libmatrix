#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"

char* Matrix_to_string(Matrix* m, int32_t hightlight_row, int32_t hightlight_col);
