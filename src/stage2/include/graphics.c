#include "./graphics.h"

uint8_t default_color[2] = {0x00, 0x0F};

uint32_t curx = 0, cury = 0;

#define COLOR_DEPTH 1


uint8_t backbuff[WIDTH * HEIGHT * COLOR_DEPTH];

void putpixel(uint32_t x, uint32_t y, uint8_t color) {
    if (x > WIDTH || y > HEIGHT) return;
    uint32_t offset = y * WIDTH + x;
    backbuff[offset] = color; 
}

void init_screen(uint8_t bg_color, uint8_t fg_color) {
    default_color[0] = bg_color;
    default_color[1] = fg_color;
    memset(backbuff, default_color[0], WIDTH * HEIGHT * COLOR_DEPTH);
}

void clear_screen(void) {
    memset(backbuff, default_color[0], WIDTH * HEIGHT * COLOR_DEPTH);
}

void draw(uint8_t icon[8][8], uint8_t color, uint32_t sx, uint32_t sy) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (icon[y][x] == 1) {
                putpixel(sx + x, sy + y, color);
            }
        }
    }
}

void fill_rect(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey, uint8_t color) {
    for (int y = sy; y <ey; y++) {
        for (int x = sx; x < ex; x++) {
            putpixel(x, y, color);
        }
    }
}

void finish(void) {
    memcpy((uint8_t*)VGA_MEMORY, backbuff, WIDTH * HEIGHT * COLOR_DEPTH);
}

static void convert_font_data(uint8_t character, uint8_t icon[8][8]) {
    for (int i = 0; i < 8; i++) {
        uint8_t byte = g_8x8_font[character * 8 + i];
        for (int j = 0; j < 8; j++) {
            icon[i][j] = (byte >> (7 - j)) & 1;
        }
    }
}

void write_char(uint8_t chr, uint8_t color, uint32_t x, uint32_t y) {
    if (chr == '\b') {
        for (int y = cury; y < cury+8; y++) {
            for (int x = curx; x < curx+8; x++) {
                putpixel(x, y, default_color[0]);
            }
        }
        finish();
        return;
    }
    uint8_t icon[8][8];
    convert_font_data(chr, icon);
    draw(icon, color, x, y);
    finish();
}

void write_string(char *str, uint8_t color) {
    
    while (*str) {
        if (curx >= WIDTH) {
            curx = 0;
            cury += 8;
        }
        if (*str == '\n') {
            curx = 0;
            cury += 8;
            str++;
            continue;
        }
        write_char(*str, color, curx, cury);
        str++;
        curx+=8;
    }
    
}

void v_putchar(char c) {
    // check for special characters line \n before drawing
    if (c == '\n') {
        curx = 0;
        cury += 8;
        return;
    }
    if (curx >= WIDTH) {
        curx = 0;
        cury += 8;
    }
    
    write_char(c, default_color[1], curx, cury);
    curx += 8;
}

unsigned char g_8x8_font[2048] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E,
	0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E,
	0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,
	0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00,
	0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x92, 0x10, 0x7C,
	0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C,
	0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,
	0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF,
	0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,
	0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF,
	0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,
	0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,
	0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,
	0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0,
	0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,
	0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00,
	0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, 
	0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 
	0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00, 
	0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0x86, 0xFC, 
	0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 
	0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,
	0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, 
	0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00, 
	0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, 
	0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 
	0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, 
	0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,
	0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 
	0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00, 
	0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,
	0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00,
	0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00, 
	0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
	0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 
	0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,
	0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
	0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
	0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00, 
	0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00, 
	0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00, 
	0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00, 
	0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00, 
	0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00, 
	0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00,
	0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,
	0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
	0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00, 
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 
	0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, 
	0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 
	0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00,
	0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00, 
	0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 
	0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00, 
	0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 
	0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, 
	0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00, 
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00,
	0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00, 
	0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, 
	0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00, 
	0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 
	0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 
	0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,
	0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 
	0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 
	0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00, 
	0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00, 
	0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00,
	0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00, 
	0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00, 
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,
	0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, 
	0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00, 
	0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,
	0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00,
	0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
	0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,
	0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
	0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
	0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
	0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, 
	0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00,
	0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00, 
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
	0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00, 
	0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 
	0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,
	0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00, 
	0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 
	0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 
	0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0, 
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E, 
	0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00, 
	0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00,
	0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00, 
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, 
	0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00, 
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
	0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00,
	0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00, 
	0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 
	0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00, 
	0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00,
	0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x0C, 0x06, 0x7C, 
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
	0x7E, 0x81, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00,
	0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
	0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
	0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0x7C, 0xC6, 0xC0, 0x78, 0x0C, 0x38,
	0x7E, 0x81, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00,
	0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
	0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00, 
	0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00, 
	0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00, 
	0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00,
	0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,
	0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00, 
	0x78, 0x84, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
	0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
	0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
	0x78, 0x84, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
	0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
	0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 
	0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 
	0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18, 
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00, 
	0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x30, 0xFC, 0x30, 
	0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3, 
	0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70, 
	0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
	0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
	0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
	0x00, 0xF8, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0x00, 
	0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00, 
	0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00, 
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,
	0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00, 
	0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00, 
	0xC6, 0xCC, 0xD8, 0x36, 0x6B, 0xC2, 0x84, 0x0F, 
	0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,
	0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x18, 0x00, 
	0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,
	0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 
	0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 
	0xDB, 0xF6, 0xDB, 0x6F, 0xDB, 0x7E, 0xD7, 0xED, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 
	0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00, 
	0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 
	0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36, 
	0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,
	0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,
	0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0,
	0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,
	0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
	0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00,
	0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,
	0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,
	0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,
	0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC,
	0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,
	0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00,
	0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,
	0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,
	0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,
	0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
	0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,
	0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,
	0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,
	0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,
	0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,
	0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,
	0x58, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,
	0x70, 0x98, 0x30, 0x60, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};