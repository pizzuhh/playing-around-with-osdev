#pragma once
#include "libc/stdint.h"

#define halt asm("cli;hlt")
#define outb(port, value) \
    asm volatile ("outb %b0, %w1" : : "a"(value), "Nd"(port))

#define inb(port) ({ \
    unsigned char _val; \
    asm volatile ("inb %w1, %b0" : "=a"(_val) : "Nd"(port)); \
    _val; \
})

