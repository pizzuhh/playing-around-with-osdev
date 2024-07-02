#include "string.h"


size_t strlen(const char* str)  {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

bool strcmp(const char *a, const char *b) {
    while (*a != '\0' || *b != '\0') {
        if(*a != *b) return 1;
        a++;b++;
    }
    return 0;
}

void memset(void *dest, uint8_t c, size_t size) {
    uint8_t *ptr = dest;
    for (size_t i = 0; i <= size; ++i) {
        ptr[i] = c;
    }
}
