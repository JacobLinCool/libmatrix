#pragma once

#define RESET_FOREGROUND "\x1b[39m"
#define RESET_BACKGROUND "\x1b[49m"

#include "../component.h"
#include "../key.h"
#include "../matrix.h"
#include "../tui.h"

static inline char* component_submatrix(Matrix* m, bool* rows, bool* cols, size_t current_row,
										size_t current_col, char* help) {
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
		malloc(size + label_space + (2 * (row * col + 2)) * sizeof("\x1b[100m\x1b[49m") + 1);
	char* ptr = str;

	ptr += sprintf(ptr, "%s ", m->name);
	ptr += sprintf(ptr, "\x1b[90m");

	for (uint32_t i = 1; i <= col; ++i) {
		if (cols[i - 1]) {
			ptr += sprintf(ptr, "\x1b[103m");
		}
		ptr += sprintf(ptr, "%*" PRIu32 " ", max_width, i);
		if (cols[i - 1]) {
			ptr += sprintf(ptr, RESET_BACKGROUND);
		}
	}
	ptr += sprintf(ptr, RESET_FOREGROUND "\n");

	for (uint32_t i = 0; i < row; ++i) {
		if (rows[i]) {
			ptr += sprintf(ptr, "\x1b[103m");
		}

		ptr += sprintf(ptr, "\x1b[90m%*" PRIu32 " " RESET_FOREGROUND RESET_BACKGROUND,
					   (int)name_len, i + 1);

		for (uint32_t j = 0; j < col; ++j) {
			if (current_row == i && current_col == j) {
				ptr += sprintf(ptr, "\x1b[105m");
			} else if (cols[j] && rows[i]) {
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

			if ((current_row == i && current_col == j) || (cols[j] && rows[i])) {
				ptr += sprintf(ptr, RESET_BACKGROUND);
			}
		}

		ptr += sprintf(ptr, "\n");
	}

	struct Box box = new (Box, -4, -4);
	box.align.horizontal = ALIGN_CENTER;
	box.align.vertical = ALIGN_MIDDLE;

	char* rendered = Box_render(&box, str);
	free(str);

	struct Box help_box = new (Box, -4, 2);
	help_box.align.horizontal = ALIGN_CENTER;
	help_box.align.vertical = ALIGN_BOTTOM;
	char* help_rendered = Box_render(&help_box, help);

	char* final = malloc(strlen(rendered) + strlen(help_rendered) + 2);
	sprintf(final, "%s\n%s", help_rendered, rendered);

	free(rendered);
	free(help_rendered);

	struct Box final_box = new (Box, 0, 0);
	final_box.border = "\x1b[103m \x1b[49m";
	final_box.border_width = 1;
	final_box.double_x_border = true;

	char* final_rendered = Box_render(&final_box, final);
	free(final);

	return final_rendered;
}
