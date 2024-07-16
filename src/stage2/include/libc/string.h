#ifndef STR_H
#define STR_H
#include "./stdint.h"
#include "./stdbool.h"
size_t strlen(const char *str);
bool strcmp(const char *a, const char *b);
void *memset(void *dest, uint8_t c, size_t size);
char *strcpy(char *dest, char *src);
char *strcat(char *dest, char *src);
#endif
