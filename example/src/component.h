#pragma once

#define Component(name, args...)                                                                   \
	({                                                                                             \
		char* str = component_##name(args);                                                        \
		printf("%s", str);                                                                         \
		free(str);                                                                                 \
	})
