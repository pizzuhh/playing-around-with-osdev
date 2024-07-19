#include "stdio.h"
#include "stdbool.h"


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
extern uint8_t graphics_mode;



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
    return;
}

static char *convert(unsigned int num, int base, int pad_width, char pad_char) {
    static char Representation[] = "0123456789ABCDEF";
    static char buffer[1024];
    int len = 0;
    char *ptr;

    ptr = &buffer[1023];
    *ptr = '\0';

    do {
        *--ptr = Representation[num % base];
        len++;
        num /= base;
    } while (num != 0);
    while (len < pad_width && pad_width > 0) {
        *--ptr = pad_char;
        len++;
    }
    return ptr;
}

static void vsprintf(char *s, const char *fmt, va_list list) {
    /*
    0 - normal state - print the character
    1 - parse state - parse the % stuff
    */
    int state = 0;
    char pad_char = ' ';
    int pad_width = 0;
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
                *s++ = *fmt;
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
                        arg_s = convert(arg_i, 16, pad_width, pad_char);
                        while (*arg_s)
                            *s++ = *arg_s++;
                        break;
                    case 'd':
                    case 'h':
                    case 'i':
                        arg_i = va_arg(list, int);
                        arg_s = convert(arg_i, 10, pad_width, pad_char);
                        while (*arg_s)
                            *s++ = *arg_s++;
                        break;
                    case 'c':
                        arg_i = va_arg(list, int);
                        *s++ = arg_i;
                        break;
                    case 's':
                        arg_s = va_arg(list, char*);
                        while (*arg_s)
                            *s++ = *arg_s++;
                        break;
                    default:
                        break;
                }
                state = 0;
                break;
        }
        fmt++;
    }
    update_cursor(terminal_column, terminal_row);
}

static void printt(const char *fmt, va_list list) {
    char buffer[1024];
    vsprintf(buffer, fmt, list);
    terminal_writestring(buffer);
    memset(buffer, 0, 1024);
}

void prints(const char *fmt, ...) {
    char buffer[1024];
    va_list list;
    va_start(list, fmt);
    vsprintf(buffer, fmt, list);
    write_string_serial(buffer);
    memset(buffer, 0, 1024);
    va_end(list);
}

static void printv(uint8_t color, const char *fmt, va_list list) {
    char buffer[1024];
    vsprintf(buffer, fmt, list);
    write_string(buffer, color);
    memset(buffer, 0, 1024);
}

void printk(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    if (graphics_mode == 1)
        printv(default_color[1], fmt, list);
    else
        printt(fmt, list);
}


void putchar(const char c) {
    if (graphics_mode == 1)
        v_putchar(c);
    else
        terminal_putchar(c, terminal_color);
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


