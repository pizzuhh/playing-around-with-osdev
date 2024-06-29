#pragma once
#include "libc/stdint.h"

#define halt asm("cli;hlt")

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

void io_wait();

void disable_cursor();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
/* void update_cursor(int x, int y); */
