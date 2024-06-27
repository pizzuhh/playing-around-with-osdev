#pragma once
#include "./stdint.h"

void write_char(char c, uint16_t offset) {
   *((char*)0xB8000 + offset) = c;
}

void write_string(char *str) {
    int off = 0;
    while (*str) {
        write_char(*str, off);
        str++;
        off += 2;
    }
}
