#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include "format.h"
#include "key.h"
#include "tui.h"

/** menu controller, represents a menu, its options, its key bindings, and the selected option. */
struct MenuController {
	char*  question;
	char** options;
	size_t options_len;
	/** This will be changed after user interaction, the inital value means the number displayed for
	 * the first item.
	 */
	int32_t selected;
	int32_t key_up;
	int32_t key_down;
	int32_t key_enter;
	int32_t key_exit;
	bool	filterable;
};

/** Create a new menu controller with default key bindings. */
struct MenuController MenuController_new();

/** Display a menu and return the state of the selection.
 * @param menu a menu controller to use
 * @return true if the user selected an option
 * @return false if the user exited the menu
 */
bool show_menu(struct MenuController* menu);

struct MatrixEditor {
	Matrix* matrix;
	int32_t selected_row;
	int32_t selected_col;
	int32_t key_up;
	int32_t key_down;
	int32_t key_right;
	int32_t key_left;
	int32_t key_exit;
	int32_t key_clear;
};

struct MatrixEditor MatrixEditor_new();

bool show_matrix_editor(struct MatrixEditor* editor);

struct MatrixRowColSelector {
	Matrix* matrix;
	bool*	rows;
	bool*	cols;
	int32_t key_up;
	int32_t key_down;
	int32_t key_right;
	int32_t key_left;
	int32_t key_toggle_row;
	int32_t key_toggle_col;
	int32_t key_done;
	int32_t key_exit;
};

struct MatrixRowColSelector MatrixRowColSelector_new();

bool show_matrix_row_col_selector(struct MatrixRowColSelector* selector);
