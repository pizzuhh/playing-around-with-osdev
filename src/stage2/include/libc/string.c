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

char *strcpy(char *dest, char *src) {
    char *p = dest;
    while ((*dest++ = *src++) != '\0');
    return p;
}

char *strcat(char *dest, char *src) {
    char *p = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != '\0');
    return p;
}


void *memset(void *ptr, uint8_t c, size_t size) {
    uint8_t *p = ptr;
    while (size--) {
        *p++ = c;
    }
    return ptr;
}
