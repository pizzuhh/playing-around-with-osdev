#include "stdio.h"
#include <stdarg.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}


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

void terminal_clear(void) {
	terminal_row = 0;
	terminal_column = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
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
	
}

void terminal_putchar(char c, uint8_t color) 
{
	if (terminal_row >= VGA_HEIGHT) {
		terminal_clear();
	}
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
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

void printf(const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	int arg_i = 0;
	char *arg_s;
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case '%':
					terminal_putchar('%', terminal_color);
					break;
				case 'c':
					arg_i = va_arg(list, int);
					terminal_putchar(arg_i, terminal_color);
                    break;
				case 's':
					arg_s = va_arg(list, char*);
					terminal_writestring(arg_s);
                    break;
				case 'd':
					arg_i = va_arg(list, int);
					terminal_writestring(convert(arg_i, 10));
                    break;
				case 'x':
				case 'X':
					arg_i = va_arg(list, int);
					terminal_writestring(convert(arg_i, 16));
				default:
					break;
			}
		} else {
			terminal_putchar(*fmt, terminal_color);
		}
		fmt++;
	}
	va_end(list);
	update_cursor(terminal_column, terminal_row);
}

char *convert(unsigned int num, int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
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
