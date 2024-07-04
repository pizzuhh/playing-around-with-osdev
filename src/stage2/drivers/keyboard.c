#include "keyboard.h"
#include "../include/libc/stdint.h"

char *__qwertyuiop = "qwertyuiop";
char *__asdfghjkl = "asdfghjkl";
char *__zxcvbnm = "zxcvbnm";
char *__num = "123456789";

char last_pressed_key = '\0';
char keycache[256];
uint8_t kci = 0;

void put_kb(uint8_t key) {
    if (key == SQ_BRACKETO_PRESSED) {
        terminal_putchar('[', terminal_color); 
        last_pressed_key = '[';
    } else if (key == SQ_BRACKETC_PRESSED) {
        terminal_putchar(']', terminal_color);
        last_pressed_key = ']';
    } else if (key == ENTER_PRESSED) {
        terminal_putchar('\n', terminal_color);
        last_pressed_key = '\n';
    } else if (key == BACKSPACE_PRESSED) {
        // TODO: Make this better
        if (terminal_column > 0)
            terminal_putentryat(' ', terminal_color, --terminal_column, terminal_row);
        else
            terminal_putentryat(' ', terminal_color, terminal_column = VGA_WIDTH - 1, --terminal_row);
        keycache[kci--] = '\0'; // update the keycache so we get valid input if we ask the user
    } else if (key == SEMICOLON_PRESSED) {
        terminal_putchar(';', terminal_color);
        last_pressed_key = ';';
    } else if (key == SINGLE_QUOTE_PRESSED) {
        terminal_putchar('\'', terminal_color);
        last_pressed_key = '\'';
    } else if (key == BACK_TICK_PRESSED) {
        terminal_putchar('`', terminal_color);
        last_pressed_key = '`';
    } else if (key == BACK_SLASH_PRESSED) {
        terminal_putchar('\\', terminal_color);
        last_pressed_key = '\\';
    } else if (key == COMMA_PRESSED) {
        terminal_putchar(',', terminal_color);
        last_pressed_key = ',';
    } else if (key == DOT_PRESSED) {
        terminal_putchar('.', terminal_color);
        last_pressed_key = '.';
    } else if (key == KP_STAR_PRESSED) {
        terminal_putchar('*', terminal_color);
        last_pressed_key = '*';
    } else if (key == SPACE_PRESSED) {
        terminal_putchar(' ', terminal_color);
        last_pressed_key = ' ';
    } else if (key == ZERO_PRESSED) {
        terminal_putchar('0', terminal_color);
        last_pressed_key = '0';
    } else if (key == KP_PLUS_PRESSED) {
        terminal_putchar('+', terminal_color);
        last_pressed_key = '+';
    } else if (key == KP_MINUS_PRESSED) {
        terminal_putchar('-', terminal_color);
        last_pressed_key = '-';
    } else if (key == EQUAL_PRESSED) {
        terminal_putchar('=', terminal_color);
        last_pressed_key = '=';
    } else if (key == DASH_PRESSED)  {
        terminal_putchar('-', terminal_color);
        last_pressed_key = '-';
    } else if (key == FORWARD_SLASH_PRESSED) {
        terminal_putchar('/', terminal_color);
        last_pressed_key = '/';
    } else if (key == F12_PRESSED) {
        asm volatile ("jmp 0xFFFF");
    } else if (key >= ONE_PRESSED && key <= NINE_PRESSED){
        terminal_putchar(__num[key - ONE_PRESSED], terminal_color);
        last_pressed_key = __num[key - ONE_PRESSED];
    } else if (key >= Q_PRESSED && key <= P_PRESSED){
        terminal_putchar(__qwertyuiop[key - Q_PRESSED], terminal_color);
        last_pressed_key = __qwertyuiop[key - Q_PRESSED];
    } else if (key >= A_PRESSED && key <= L_PRESSED) {
        terminal_putchar(__asdfghjkl[key - A_PRESSED], terminal_color);
        last_pressed_key = __asdfghjkl[key - A_PRESSED];
    } else if (key >= Z_PRESSED && key <= M_PRESSED) {
        terminal_putchar(__zxcvbnm[key - Z_PRESSED], terminal_color);
        last_pressed_key = __zxcvbnm[key - Z_PRESSED];
    }
}


__attribute__((interrupt)) void kbd_handler(interrupt_frame *frame) {
    uint8_t code = inb(0x60);
    put_kb(code);
    if (last_pressed_key != '\0') {
        if (kci >= 255) kci = 0; // Prevent overflow. We probably wont need the last 256 keys lol
        keycache[kci] = last_pressed_key;
        kci++;
        last_pressed_key = '\0';
    }
    PIC_sendEOI(1);
}



char *get_input(void) {
    char *ret = (char*)0x55000; // !!!! THIS IS BAD !!!!
    uint8_t i = 0;
     while (1) {
        if (keycache[i] == '\n') {
            ret[i] = '\0';
            kci = 0; // Reset key cache index
            keycache[i] = 0x00; // remove the new line since the next call won't work
            return ret;
        }
        ret[i] = keycache[i];
        i++;
    }
    return ret;
}
