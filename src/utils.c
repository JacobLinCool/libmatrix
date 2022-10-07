#include "utils.h"

char* random_name(u32 length) {
	char* str = malloc(sizeof(char) * length);
	for (u32 i = 0; i < length; i++) {
		str[i] = (char)(rand() % 26 + 97);
	}
	return str;
}
