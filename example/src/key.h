#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

enum KEY {
	KEY_NONE = 0,
	KEY_ENTER = '\n',
	KEY_SPACE = ' ',
	KEY_TAB = '\t',
	KEY_BACKSPACE = 127,
	KEY_ESCAPE = '\e',
	KEY_UP = 458948673,
	KEY_DOWN = 458948674,
	KEY_RIGHT = 458948675,
	KEY_LEFT = 458948676,
};

int	  take_keypress();
char* visialize_key(int32_t key);
