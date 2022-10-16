#include "tui.h"

struct Box Box_new(int32_t width, int32_t height) {
	struct Box box = {.width = width,
					  .height = height,
					  .align =
						  {
							  .horizontal = ALIGN_LEFT,
							  .vertical = ALIGN_TOP,
						  },
					  .border_width = 0,
					  .border = NULL,
					  .double_x_border = false};

	if (width <= 0 || height <= 0) {
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		if (width <= 0) {
			box.width = w.ws_col + width;
			box.width = box.width < 0 ? w.ws_col : box.width;
		}

		if (height <= 0) {
			box.height = w.ws_row + height;
			box.height = box.height < 0 ? w.ws_row : box.height;
		}
	}

	return box;
}

struct Box Box_root() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return Box_new(w.ws_col, w.ws_row);
}

char* Box_render(struct Box* box, const char* content) {
	char* rendered =
		malloc(box->height * (box->width + 1) * (sizeof(char) + sizeof("\x1b[100;90m\x1b[0m")));
	char* ptr = rendered;

	int32_t border_width_x = box->border_width;
	int32_t border_width_y = box->border_width;

	if (box->double_x_border) {
		border_width_x <<= 1;
	}

	if (border_width_y > box->height / 2) {
		border_width_y = box->height / 2;
	}

	if (border_width_x > box->width / 2) {
		border_width_x = box->width / 2;
	}

	int32_t height_available = box->height - border_width_y * 2;
	int32_t width_available = box->width - border_width_x * 2;

	int32_t content_len = strlen(content);
	int32_t idx = 0;

	struct {
		char*	line;
		int32_t left, right;
	}* lines = malloc(sizeof(struct {
						  char*	  line;
						  int32_t left, right;
					  }) *
					  height_available);
	int32_t visible_lines = 0;

	for (int32_t i = 0; i < height_available; ++i) {
		char*	line = malloc(width_available * (sizeof(char) + sizeof("\x1b[100;90m\x1b[0m")));
		char*	line_ptr = line;
		int32_t visible_length = 0;

		while (idx < content_len && visible_length <= width_available) {
			if (content[idx] == '\x1b') {
				while (idx < content_len && content[idx] != 'm') {
					line_ptr += sprintf(line_ptr, "%c", content[idx++]);
				}
				line_ptr += sprintf(line_ptr, "%c", content[idx++]);
			} else if (content[idx] == '\n') {
				++idx;
				break;
			} else if (visible_length < width_available) {
				line_ptr += sprintf(line_ptr, "%c", content[idx++]);
				++visible_length;
			} else {
				break;
			}
		}

		int32_t left = 0, right = 0;
		if (box->align.horizontal == ALIGN_LEFT) {
			right = width_available - visible_length;
		} else if (box->align.horizontal == ALIGN_CENTER) {
			left = (width_available - visible_length) / 2;
			right = width_available - visible_length - left;
		} else if (box->align.horizontal == ALIGN_RIGHT) {
			left = width_available - visible_length;
		}

		if (visible_length > 0) {
			lines[visible_lines].line = line;
			lines[visible_lines].left = left;
			lines[visible_lines].right = right;
			++visible_lines;
		}
	}

	int32_t top = (height_available - visible_lines) / 2;
	int32_t bottom = height_available - visible_lines - top;

	if (box->border != NULL) {
		for (int32_t i = 0; i < border_width_y; ++i) {
			for (int32_t j = 0; j < box->width; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
			if (i != border_width_y - 1) {
				ptr += sprintf(ptr, "\n");
			}
		}
	}

	if (top > 0) {
		ptr += sprintf(ptr, "\n");
	}
	for (int32_t i = 0; i < top; ++i) {
		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		ptr += sprintf(ptr, "%*s", width_available, "");

		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		if (i != top - 1) {
			ptr += sprintf(ptr, "\n");
		}
	}

	if (visible_lines > 0) {
		ptr += sprintf(ptr, "\n");
	}
	for (int32_t i = 0; i < visible_lines; ++i) {
		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		ptr += sprintf(ptr, "%*s", lines[i].left, "");

		ptr += sprintf(ptr, "%s", lines[i].line);
		free(lines[i].line);

		ptr += sprintf(ptr, "%*s", lines[i].right, "");

		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		if (i != visible_lines - 1) {
			ptr += sprintf(ptr, "\n");
		}
	}

	free(lines);

	if (bottom > 0) {
		ptr += sprintf(ptr, "\n");
	}
	for (int32_t i = 0; i < bottom; ++i) {
		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		ptr += sprintf(ptr, "%*s", width_available, "");

		if (box->border != NULL) {
			for (int32_t j = 0; j < border_width_x; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
		}

		if (i != bottom - 1) {
			ptr += sprintf(ptr, "\n");
		}
	}

	if (box->border != NULL) {
		ptr += sprintf(ptr, "\n");
		for (int32_t i = 0; i < border_width_y; ++i) {
			for (int32_t j = 0; j < box->width; ++j) {
				ptr += sprintf(ptr, "%s", box->border);
			}
			if (i != border_width_y - 1) {
				ptr += sprintf(ptr, "\n");
			}
		}
	}

	rendered = realloc(rendered, ptr - rendered + 1);
	return rendered;
}

void clear_screen() {
	printf("\x1b[2J\x1b[1;1H");
	fflush(stdout);
}
void set_cursor_pos(int32_t x, int32_t y) { printf("\x1b[%d;%dH", y, x); }
void set_cursor_visibility(bool visible) { printf("\x1b[?25%c", visible ? 'h' : 'l'); }
void set_cursor_blink(bool blink) { printf("\x1b[?12%c", blink ? 'h' : 'l'); }
