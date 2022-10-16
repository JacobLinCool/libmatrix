#pragma once

#include "../component.h"
#include "../tui.h"

static inline char* component_welcome() {
	struct Box header_box = new (Box, -4, 6);
	header_box.align.horizontal = ALIGN_CENTER;
	header_box.align.vertical = ALIGN_MIDDLE;
	struct Box content_box = new (Box, -4, -8);
	content_box.align.horizontal = ALIGN_CENTER;
	content_box.align.vertical = ALIGN_MIDDLE;

	char* header =
		"Welcome to Matrix Calculator\n \n"
		"Author: Jacob Lin <hi@jacoblin.cool>\n"
		"(https://github.com/JacobLinCool)";

	char* content =
		"\x1b[93mPress \x1b[103;30m ANY KEY \x1b[49;93m to launch the calculator\x1b[39m";

	char* header_str = Box_render(&header_box, header);
	char* content_str = Box_render(&content_box, content);

	char* result = malloc(strlen(header_str) + strlen(content_str) + 1);
	strcpy(result, header_str);
	strcat(result, content_str);
	free(header_str);
	free(content_str);

	struct Box full_box = new (Box, 0, 0);
	full_box.border = "\x1b[106m \x1b[49m";
	full_box.border_width = 1;
	full_box.double_x_border = true;

	char* full_str = Box_render(&full_box, result);
	free(result);

	return full_str;
}
