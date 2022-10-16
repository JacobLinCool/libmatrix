#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

enum ContentHorizontalAlignment {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

enum ContentVerticalAlignment {
	ALIGN_TOP,
	ALIGN_MIDDLE,
	ALIGN_BOTTOM,
};

/** A box is a rectangular area that can be used to render text. */
struct Box {
	int32_t width, height;
	struct {
		enum ContentHorizontalAlignment horizontal;
		enum ContentVerticalAlignment	vertical;
	} align;

	const char* border;
	int32_t		border_width;
	bool		double_x_border;
};

/**
 * Create a new display box.
 *
 * @param width The width of the box. If non-positive, the width will be set to `terminal width +
 * width`.
 * @param height The height of the box. If non-positive, the height will be set to `terminal height
 * + height`.
 * @return The new display box.
 */
struct Box Box_new(int32_t width, int32_t height);

/**
 * Create a new display box that fills the entire terminal.
 * Equivalent to `Box_new(0, 0)`.
 * @return The full size display box.
 */
struct Box Box_root();

/**
 * Render a text within a box.
 * @param box The box to render the text in.
 * @param content The text to render.
 * @return The rendered text.
 */
char* Box_render(struct Box* box, const char* content);

void clear_screen();
void set_cursor_pos(int32_t x, int32_t y);
void set_cursor_visible(bool visible);
void set_cursor_blink(bool blink);

#ifndef new
// clang-format off
/** Create a new instance of a class-like structure. */
#define new(_name, args...) _name##_new(args)
// clang-format on
#endif
