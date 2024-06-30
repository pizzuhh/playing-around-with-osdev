#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../include/libc/stdio.h"
#include "../include/libc/stdint.h"
#include "../include/libc/stdbool.h"
#include "../include/idt.h"
#include "../include/io.h"
#include "../include/pic.h"

__attribute__((interrupt)) void kbd_handler(interrupt_frame *frame);

// TODO: Keyboard driver
#endif
