#include "key.h"

int32_t take_keypress() {
	struct termios attr;
	struct termios tmp_attr;

	tcgetattr(fileno(stdin), &attr);
	tmp_attr = attr;
	tmp_attr.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(fileno(stdin), TCSANOW, &tmp_attr);
	setbuf(stdin, NULL);

	struct timeval tv = {.tv_sec = 0, .tv_usec = 1000};

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fileno(stdin), &fds);

	int32_t key_code = getchar();

	int offset = 1;
	while (select(fileno(stdin) + 1, &fds, NULL, NULL, &tv) == 1) {
		if (key_code < 1 << 16) {
			key_code = (key_code << 8 * offset++) | getchar();
		} else {
			break;
		}
	}

	tcsetattr(fileno(stdin), TCSANOW, &attr);

	return key_code;
}

char* visialize_key(int32_t key) {
	if (key == KEY_ENTER) {
		return strdup("enter");
	} else if (key == KEY_SPACE) {
		return strdup("space");
	} else if (key == KEY_TAB) {
		return strdup("tab");
	} else if (key == KEY_BACKSPACE) {
		return strdup("backspace");
	} else if (key == KEY_ESCAPE) {
		return strdup("escape");
	} else if (key <= KEY_NONE) {
		return strdup("none");
	} else if (key == KEY_UP) {
		return strdup("up");
	} else if (key == KEY_DOWN) {
		return strdup("down");
	} else if (key == KEY_RIGHT) {
		return strdup("right");
	} else if (key == KEY_LEFT) {
		return strdup("left");
	} else {
		char* str = malloc(2 * sizeof(char));
		str[0] = key;
		str[1] = '\0';
		return str;
	}
}
