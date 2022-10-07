#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	srand(1481);

	char* name = random_name(4);
	assert(strcmp(name, "ptyz") == 0);

	return EXIT_SUCCESS;
}
