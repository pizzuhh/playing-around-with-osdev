#ifndef SERIAL_H
#define SERIAL_H
#define COM1 0x3f8

#include "../include/io.h"
#include "../include/libc/stdint.h"
#include "../include/libc/string.h"
#include <stdarg.h>

int init_serial(void);
void write_char_serial(char c);
void write_string_serial(char *str);
// same as printf but prints to serial.
void prints(const char *fmt, ...);
#endif
