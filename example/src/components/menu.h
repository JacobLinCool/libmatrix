#pragma once

#include "../component.h"
#include "../key.h"
#include "../tui.h"

static inline char* component_menu(struct MenuController* controller, char* filter,
								   int32_t selected) {
	struct Box title_box = new (Box, -4, 4);
	title_box.align.horizontal = ALIGN_CENTER;
	title_box.align.vertical = ALIGN_MIDDLE;
	struct Box option_box = new (Box, -4, -7 - controller->filterable);
	option_box.align.horizontal = ALIGN_CENTER;
	option_box.align.vertical = ALIGN_MIDDLE;
	struct Box help_box = new (Box, -4, 1);

	char* header = malloc(strlen(controller->question) + sizeof("\x1b[96m\x1b[39m") + 1);
	sprintf(header, "\x1b[96m%s\x1b[39m", controller->question);

	char* option = malloc(option_box.width * option_box.height * sizeof(char) * 10);
	char* option_ptr = option;

	for (size_t i = 0; i < controller->options_len; ++i) {
		if (controller->filterable && strstr(controller->options[i], filter) == NULL) {
			option_ptr += sprintf(option_ptr, " \x1b[90m");
		} else if ((int32_t)i + controller->selected == selected) {
			option_ptr += sprintf(option_ptr, " \x1b[93m");
		} else {
			option_ptr += sprintf(option_ptr, " \x1b[97m");
		}
		// option_ptr +=
		// 	sprintf(option_ptr, (int32_t)i + controller->selected == selected ? "> " : "  ");
		option_ptr += sprintf(option_ptr, " %s \x1b[m\n", controller->options[i]);
	}

	char* help = malloc(help_box.width * help_box.height * sizeof(char) * 10);
	char* up_key = visialize_key(controller->key_up);
	char* down_key = visialize_key(controller->key_down);
	char* exit_key = visialize_key(controller->key_exit);
	char* enter_key = visialize_key(controller->key_enter);
	sprintf(help,
			"\nUse \"\x1b[95m%s\x1b[m\" and \"\x1b[95m%s\x1b[m\" to navigate, \"\x1b[95m%s\x1b[m\" "
			"to exit, \"\x1b[95m%s\x1b[m\" to select.\n",
			up_key, down_key, exit_key, enter_key);
	free(up_key);
	free(down_key);
	free(exit_key);
	free(enter_key);

	struct Box filter_box = new (Box, -4, 1);
	char*	   filter_str = malloc(strlen(filter) + sizeof("\x1b[93m\x1b[39m") + 1);
	sprintf(filter_str, "Filter: \x1b[93m%s\x1b[39m", filter);

	char* title_str = Box_render(&title_box, header);
	char* option_str = Box_render(&option_box, option);
	char* help_str = Box_render(&help_box, help);
	char* rendered_filter_str = Box_render(&filter_box, filter_str);

	char* result = malloc(strlen(title_str) + strlen(option_str) + strlen(help_str) +
						  strlen(rendered_filter_str) + 1);
	strcpy(result, title_str);
	strcat(result, option_str);
	if (controller->filterable) {
		strcat(result, rendered_filter_str);
	}
	strcat(result, help_str);

	free(title_str);
	free(option_str);
	free(help_str);
	free(rendered_filter_str);

	free(header);
	free(option);
	free(help);
	free(filter_str);

	struct Box full_box = new (Box, 0, 0);
	full_box.border = "\x1b[106m \x1b[49m";
	full_box.border_width = 1;
	full_box.double_x_border = true;

	char* full_str = Box_render(&full_box, result);
	free(result);

	return full_str;
}
