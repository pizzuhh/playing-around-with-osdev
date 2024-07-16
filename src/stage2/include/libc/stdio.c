#include "stdio.h"
#include "stdbool.h"
#include <stdarg.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;




void terminal_initialize(uint8_t color) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = color;
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_clear(uint8_t color) {
	terminal_row = 0;
	terminal_column = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t col, size_t row) 
{
	const size_t index = row * VGA_WIDTH + col;
	terminal_buffer[index] = vga_entry(c, color);
	update_cursor(terminal_column, terminal_row);
}

void terminal_putchar(char c, uint8_t color) 
{
	if (terminal_row >= VGA_HEIGHT) {
		terminal_clear(terminal_color);
	}
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
		update_cursor(terminal_column, terminal_row);
        return;
    }
	// go back to top if we reach end
    terminal_putentryat(c, color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
	update_cursor(terminal_column, terminal_row);
}



void terminal_write(const char* data, size_t size, uint8_t color) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i], color);
}

void terminal_writestring_color(const char* data, uint8_t color) 
{
	terminal_write(data, strlen(data), color);
}
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data), terminal_color);
	update_cursor(terminal_column, terminal_row);
}

static void print_str(char *str, char pad_char, int width) {
    if (width <= 0) {
        terminal_writestring(str);
        return;
    }
    char buffer[1024];
    memset(buffer, pad_char, 1024);
    strcpy(buffer + width, str);
    terminal_writestring(buffer);
    return;
}

void printf(const char *fmt, ...) {
    /*
    0 - normal state - print the character
    1 - parse state - parse the % stuff
    */
    int state = 0;
    char pad_char = ' ';
    int pad_width = 0;
    va_list list;
    va_start(list, fmt);
    int arg_i;
    char *arg_s;
    while (*fmt) {
        if (*fmt == '%') {
            state = 1;
            pad_char = ' ';
            pad_width = 0;
            fmt++;
        }
        switch (state) {
            default:
            case 0:
                terminal_putchar(*fmt, terminal_color);
                break;
            case 1:
                state = 0;
                if (*fmt == '0') {pad_char = '0';fmt++;} 
                while (*fmt >= '0' && *fmt <= '9') {
                    pad_width = pad_width * 10 + (*fmt - '0');
                    fmt++;
                }
                switch (*fmt) {
                    case 'x':
                    case 'X':
                        arg_i = va_arg(list, int);
                        print_str(convert(arg_i, 16), pad_char, pad_width);
                        //fmt++;
                        break;
                    case 'd':
                    case 'h':
                    case 'i':
                        arg_i = va_arg(list, int);
                        print_str(convert(arg_i, 10), pad_char, pad_width);
                        //fmt++;
                        break;
                    case 'c':
                        arg_i = va_arg(list, int);
                        terminal_putchar(arg_i, terminal_color);
                        //fmt++;
                        break;
                    case 's':
                        arg_s = va_arg(list, char*);
                        print_str(arg_s, pad_char, pad_width);
                        //fmt++;
                        break;
                    default:
                        break;
                }
                state = 0;
                break;
        }
        fmt++;
    }
    va_end(list);
    update_cursor(terminal_column, terminal_row);
}

char *convert(unsigned int num, int base) {
    static char Representation[] = "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = Representation[num % base];
        num /= base;
    } while (num != 0);

    return ptr;
}

void printe(const char *exception) {
    terminal_write(exception, strlen(exception), 0x0F);
}
void printd(uint32_t delay, const char *msg) {
	while (*msg) {
		terminal_putchar(*msg, terminal_color);
		update_cursor(terminal_column, terminal_row);
		msg++;
		msleep(delay);
	}
	
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}


