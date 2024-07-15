#ifndef STR_H
#define STR_H
#include "./stdint.h"
#include "./stdbool.h"
size_t strlen(const char *str);
bool strcmp(const char *a, const char *b);
void memset(void *dest, uint8_t c, size_t size);
void strcpy(char *dest, char *src);
void strcat(char *dest, char *src);
#endif
