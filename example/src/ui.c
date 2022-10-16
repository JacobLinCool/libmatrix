#include "ui.h"

#include "components/menu.h"
#include "components/submatrix.h"
#include "key.h"
#define MAGIC_NUMBER -192837465

struct MenuController MenuController_new() {
	struct MenuController menu = {.question = NULL,
								  .options = NULL,
								  .options_len = 0,
								  .selected = 1,
								  .key_up = KEY_UP,
								  .key_down = KEY_DOWN,
								  .key_enter = KEY_ENTER,
								  .key_exit = KEY_ESCAPE,
								  .filterable = true};

	return menu;
}

bool show_menu(struct MenuController* menu) {
	if (menu->options_len == 0) {
		return false;
	}

	char* up_key = visialize_key(menu->key_up);
	char* down_key = visialize_key(menu->key_down);
	char* exit_key = visialize_key(menu->key_exit);
	char* enter_key = visialize_key(menu->key_enter);

	char*	filter = calloc(128, sizeof(char));
	size_t	filter_len = 0;
	int32_t selected = menu->selected;
	int32_t key = MAGIC_NUMBER;
	while (key != menu->key_enter && key != menu->key_exit) {
		clear_screen();
		printf("%s\n", menu->question);

		for (size_t i = 0; i < menu->options_len; ++i) {
			if (menu->filterable && strstr(menu->options[i], filter) == NULL) {
				printf(" \x1b[90m");
			} else if ((int32_t)i + menu->selected == selected) {
				printf(" \x1b[93m");
			} else {
				printf(" \x1b[97m");
			}
			printf((int32_t)i + menu->selected == selected ? "> " : "  ");
			printf(" %s\x1b[m\n", menu->options[i]);
		}
		printf(
			"\nUse \"\x1b[95m%s\x1b[m\" and \"\x1b[95m%s\x1b[m\" to navigate, \"\x1b[95m%s\x1b[m\" "
			"to exit, \"\x1b[95m%s\x1b[m\" to select.\n",
			up_key, down_key, exit_key, enter_key);
		printf("filter: \x1b[93m%s\x1b[m", filter);
		fflush(stdout);

		fflush(stdin);
		key = take_keypress();

		if (key == menu->key_up) {
			selected--;
			if (selected < menu->selected) {
				selected = menu->options_len - 1 + menu->selected;
			}
		} else if (key == menu->key_down) {
			selected++;
			if (selected >= (int32_t)menu->options_len + menu->selected) {
				selected = menu->selected;
			}
		} else if (key == menu->key_enter && menu->filterable &&
				   strstr(menu->options[selected - menu->selected], filter) == NULL) {
			key = MAGIC_NUMBER;
		}

		if (menu->filterable &&
			((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') ||
			 (key >= '0' && key <= '9') || key == KEY_BACKSPACE || key == KEY_SPACE)) {
			if (key == KEY_BACKSPACE) {
				if (filter_len > 0) {
					filter[--filter_len] = '\0';
				}
			} else if (filter_len < 127) {
				filter[filter_len++] = key;
			}
		}
	}

	free(up_key);
	free(down_key);
	free(exit_key);
	free(enter_key);

	if (key == menu->key_enter) {
		menu->selected = selected;
	}
	clear_screen();
	return key == menu->key_enter;
}

struct MatrixEditor MatrixEditor_new() {
	struct MatrixEditor editor = {
		.matrix = NULL,
		.selected_row = 0,
		.selected_col = 0,
		.key_up = KEY_UP,
		.key_down = KEY_DOWN,
		.key_left = KEY_LEFT,
		.key_right = KEY_RIGHT,
		.key_exit = KEY_ESCAPE,
		.key_clear = KEY_BACKSPACE,
	};

	return editor;
}

bool show_matrix_editor(struct MatrixEditor* editor) {
	if (editor->matrix == NULL) {
		return false;
	}

	char* up_key = visialize_key(editor->key_up);
	char* down_key = visialize_key(editor->key_down);
	char* left_key = visialize_key(editor->key_left);
	char* right_key = visialize_key(editor->key_right);
	char* clear_key = visialize_key(editor->key_clear);
	char* exit_key = visialize_key(editor->key_exit);

	int32_t selected_row = editor->selected_row;
	int32_t selected_col = editor->selected_col;
	int32_t key = MAGIC_NUMBER;
	bool	point = false;
	while (key != editor->key_exit) {
		clear_screen();

		char* str = Matrix_to_string(editor->matrix, selected_row, selected_col);
		printf("%s\n", str);
		free(str);

		printf(
			"Use \"\x1b[95m%s\x1b[m\" / \"\x1b[95m%s\x1b[m\" / \"\x1b[95m%s\x1b[m\" / "
			"\"\x1b[95m%s\x1b[m\" to navigate, \n\"\x1b[95m%s\x1b[m\" to clear, "
			"\"\x1b[95m%s\x1b[m\" to exit.\n",
			up_key, down_key, left_key, right_key, clear_key, exit_key);

		fflush(stdin);
		key = take_keypress();

		if (key == editor->key_up) {
			selected_row--;
			if (selected_row < 0) {
				selected_row = editor->matrix->data[0].row - 1;
			}
		} else if (key == editor->key_down) {
			selected_row++;
			if (selected_row >= (int32_t)editor->matrix->data[0].row) {
				selected_row = 0;
			}
		} else if (key == editor->key_left) {
			selected_col--;
			if (selected_col < 0) {
				selected_col = editor->matrix->data[0].col - 1;
			}
		} else if (key == editor->key_right) {
			selected_col++;
			if (selected_col >= (int32_t)editor->matrix->data[0].col) {
				selected_col = 0;
			}
		} else if (key == editor->key_clear) {
			Matrix_set(editor->matrix, selected_row, selected_col, 0);
		} else if (key >= '0' && key <= '9') {
			if (point == false) {
				double num = Matrix_get(editor->matrix, selected_row, selected_col);
				num *= 10;
				num += key - '0';
				Matrix_set(editor->matrix, selected_row, selected_col, num);
			} else {
				double	num = Matrix_get(editor->matrix, selected_row, selected_col);
				int64_t integer = (int64_t)num;
				double	decimal = num - integer;

				num = integer + ((key - '0') + decimal) / 10.0;

				Matrix_set(editor->matrix, selected_row, selected_col, num);
			}
		} else if (key == '.') {
			point = !point;
		} else if (key == '-') {
			Matrix_set(editor->matrix, selected_row, selected_col,
					   -1 * Matrix_get(editor->matrix, selected_row, selected_col));
		}

		if (key != '.' && key != '-' && (key < '0' || key > '9')) {
			point = false;
		}
	}

	free(up_key);
	free(down_key);
	free(left_key);
	free(right_key);
	free(exit_key);
	free(clear_key);

	editor->selected_row = selected_row;
	editor->selected_col = selected_col;
	clear_screen();
	return key == editor->key_exit;
}

struct MatrixRowColSelector MatrixRowColSelector_new() {
	struct MatrixRowColSelector selector = {
		.matrix = NULL,
		.rows = NULL,
		.cols = NULL,
		.key_up = KEY_UP,
		.key_down = KEY_DOWN,
		.key_left = KEY_LEFT,
		.key_right = KEY_RIGHT,
		.key_toggle_row = 'r',
		.key_toggle_col = 'c',
		.key_done = KEY_ENTER,
		.key_exit = KEY_ESCAPE,
	};

	return selector;
}

bool show_matrix_row_col_selector(struct MatrixRowColSelector* selector) {
	if (selector->matrix == NULL) {
		return false;
	}

	char* up_key = visialize_key(selector->key_up);
	char* down_key = visialize_key(selector->key_down);
	char* left_key = visialize_key(selector->key_left);
	char* right_key = visialize_key(selector->key_right);
	char* toggle_row_key = visialize_key(selector->key_toggle_row);
	char* toggle_col_key = visialize_key(selector->key_toggle_col);
	char* done_key = visialize_key(selector->key_done);
	char* exit_key = visialize_key(selector->key_exit);

	int32_t current_row = 0;
	int32_t current_col = 0;
	int32_t key = MAGIC_NUMBER;
	while (key != selector->key_done && key != selector->key_exit) {
		char* help = malloc(300);
		sprintf(help,
				"Use \"\x1b[95m%s\x1b[m\" / \"\x1b[95m%s\x1b[m\" / \"\x1b[95m%s\x1b[m\" / "
				"\"\x1b[95m%s\x1b[m\" to navigate, \"\x1b[95m%s\x1b[m\" to select row, "
				"\"\x1b[95m%s\x1b[m\" to select column, \"\x1b[95m%s\x1b[m\" to confirm, "
				"\"\x1b[95m%s\x1b[m\" to exit.\n",
				up_key, down_key, left_key, right_key, toggle_row_key, toggle_col_key, done_key,
				exit_key);

		clear_screen();
		Component(submatrix, selector->matrix, selector->rows, selector->cols, current_row,
				  current_col, help);
		free(help);

		fflush(stdin);
		key = take_keypress();

		if (key == selector->key_up) {
			current_row--;
			if (current_row < 0) {
				current_row = selector->matrix->data[0].row - 1;
			}
		} else if (key == selector->key_down) {
			current_row++;
			if (current_row >= (int32_t)selector->matrix->data[0].row) {
				current_row = 0;
			}
		} else if (key == selector->key_left) {
			current_col--;
			if (current_col < 0) {
				current_col = selector->matrix->data[0].col - 1;
			}
		} else if (key == selector->key_right) {
			current_col++;
			if (current_col >= (int32_t)selector->matrix->data[0].col) {
				current_col = 0;
			}
		} else if (key == selector->key_toggle_row) {
			if (selector->rows == NULL) {
				fprintf(stderr, "Selecting rows is not allowed\n");
				continue;
			}

			if (selector->rows[current_row] == true) {
				selector->rows[current_row] = false;
			} else {
				selector->rows[current_row] = true;
			}
		} else if (key == selector->key_toggle_col) {
			if (selector->cols == NULL) {
				fprintf(stderr, "Selecting cols is not allowed\n");
				continue;
			}

			if (selector->cols[current_col] == true) {
				selector->cols[current_col] = false;
			} else {
				selector->cols[current_col] = true;
			}
		}
	}

	free(up_key);
	free(down_key);
	free(left_key);
	free(right_key);
	free(toggle_row_key);
	free(toggle_col_key);
	free(done_key);
	free(exit_key);

	clear_screen();
	return key == selector->key_done;
}
