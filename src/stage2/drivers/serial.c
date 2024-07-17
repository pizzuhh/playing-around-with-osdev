#include "serial.h"

bool is_serial_on = false;

// https://wiki.osdev.org/Serial_Ports#Initialization
int init_serial(void) {
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(COM1 + 0) != 0xAE) {
       return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1 + 4, 0x0F);
    is_serial_on = true;
    return 0;
}

static inline int is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20;
}

void write_char_serial(char c) {
    while (is_transmit_empty() == 0);
    outb(COM1, c);
}

void write_string_serial(char *str) {
    if (!is_serial_on) {init_serial();}
    while(*str) {
        write_char_serial(*str);
        str++;
    }
}

static void print_strs(char *str, char pad_char, int width) {
    if (width <= 0) {
        write_string_serial(str);
        return;
    }
    size_t len = strlen(str);
    char buffer[(len + width) + 1];
    memset(buffer, pad_char, (len + width));
    strcpy(buffer + (width - len), str);
    buffer[(len + width) + 1] = '\0';
    write_string_serial(buffer);
    return;
}

static char *convert(unsigned int num, int base) {
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

void prints(const char *fmt, ...) {
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
            case 0: // since serial needs carriage return (\r) to move the cursor to the end of the line we put it whenever \n is encountered. No need for the user to type \r\n but just \n
                if (*fmt == '\n') {
                    write_char_serial('\r');
                    write_char_serial('\n');
                    break;
                }
                write_char_serial(*fmt);
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
                        print_strs(convert(arg_i, 16), pad_char, pad_width);
                        //fmt++;
                        break;
                    case 'd':
                    case 'h':
                    case 'i':
                        arg_i = va_arg(list, int);
                        print_strs(convert(arg_i, 10), pad_char, pad_width);
                        //fmt++;
                        break;
                    case 'c':
                        arg_i = va_arg(list, int);
                        write_char_serial(arg_i);
                        //fmt++;
                        break;
                    case 's':
                        arg_s = va_arg(list, char*);
                        print_strs(arg_s, pad_char, pad_width);
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
}
