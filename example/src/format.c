#include "format.h"

#define RESET_FOREGROUND "\x1b[39m"
#define RESET_BACKGROUND "\x1b[49m"

char* Matrix_to_string(Matrix* m, int32_t hightlight_row, int32_t hightlight_col) {
	uint32_t row = m->data[0].row;
	uint32_t col = m->data[0].col;

	int max_width = 0;
	for (uint32_t i = 0; i < row; ++i) {
		for (uint32_t j = 0; j < col; ++j) {
			int width = snprintf(NULL, 0, "%.9lg", Matrix_get(m, i, j));
			if (width > max_width) {
				max_width = width;
			}
		}
	}

	size_t name_len = strlen(m->name);
	size_t label_space = name_len * (row + 1) + (max_width + 1) * col;
	size_t size = (max_width + 1) * col * row + 1;
	char*  str =
		malloc(size + label_space + (2 * (row + col + 2)) * sizeof("\x1b[100m\x1b[49m") + 1);
	char* ptr = str;

	ptr += sprintf(ptr, "%s ", m->name);
	ptr += sprintf(ptr, "\x1b[90m");
	for (uint32_t i = 1; i <= col; ++i) {
		if ((int32_t)i == hightlight_col + 1) {
			ptr += sprintf(ptr, "\x1b[100m");
		}
		ptr += sprintf(ptr, "%*" PRIu32 " ", max_width, i);
		if ((int32_t)i == hightlight_col + 1) {
			ptr += sprintf(ptr, RESET_BACKGROUND);
		}
	}
	ptr += sprintf(ptr, RESET_FOREGROUND "\n");

	for (uint32_t i = 0; i < row; ++i) {
		if ((int32_t)i == hightlight_row) {
			ptr += sprintf(ptr, "\x1b[100m");
		}

		ptr += sprintf(ptr, "\x1b[90m%*" PRIu32 RESET_FOREGROUND " ", (int)name_len, i + 1);

		for (uint32_t j = 0; j < col; ++j) {
			if ((int32_t)i == hightlight_row && (int32_t)j == hightlight_col) {
				ptr += sprintf(ptr, "\x1b[103m");
			} else if ((int32_t)j == hightlight_col) {
				ptr += sprintf(ptr, "\x1b[100m");
			}

			double val = Matrix_get(m, i, j);
			if (val != 0) {
				ptr += sprintf(ptr, "\x1b[96m");
			}

			ptr += sprintf(ptr, "%*.9lg ", max_width, val);

			if (val != 0) {
				ptr += sprintf(ptr, RESET_FOREGROUND);
			}

			if ((int32_t)i == hightlight_row && (int32_t)j == hightlight_col) {
				ptr += sprintf(ptr, "\x1b[100m");
			} else if ((int32_t)j == hightlight_col) {
				ptr += sprintf(ptr, RESET_BACKGROUND);
			}
		}

		if ((int32_t)i == hightlight_row) {
			ptr += sprintf(ptr, RESET_BACKGROUND);
		}
		ptr += sprintf(ptr, "\n");
	}

	return str;
}
