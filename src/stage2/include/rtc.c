#include "./rtc.h"
#include "libc/stdio.h"
#include "pic.h"
#include <stdint.h>

datetime_t global_time = {0};

__attribute__((interrupt)) void rtc_handler(interrupt_frame *frame) {
    datetime_t new_time = {0}, old_time = {0};
    uint8_t regB;
    static int ticks = 0;
    asm volatile ("cli");
    ticks++;
    if (ticks % 1024 == 0) {
        while (is_update_in_progress());
        old_time.seconds = get_rtc_reg(0x00);
        old_time.minutes = get_rtc_reg(0x02);
        old_time.hours = get_rtc_reg(0x04);
        old_time.day = get_rtc_reg(0x07);
        old_time.month = get_rtc_reg(0x08);
        old_time.year = get_rtc_reg(0x09);

        do {
            new_time = old_time;
            while (is_update_in_progress());
            old_time.seconds = get_rtc_reg(0x00);
            old_time.minutes = get_rtc_reg(0x02);
            old_time.hours = get_rtc_reg(0x04);
            old_time.day = get_rtc_reg(0x07);
            old_time.month = get_rtc_reg(0x08);
            old_time.year = get_rtc_reg(0x09);
        } while((new_time.seconds != old_time.seconds) || (new_time.minutes != old_time.minutes) ||
                (new_time.hours != old_time.hours)     || (new_time.day != old_time.day)         ||
                (new_time.month != old_time.month)     || (new_time.year != old_time.year));
        regB = get_rtc_reg(0x0B);
        if (!(regB & 0x04)) {
            new_time.seconds = (new_time.seconds & 0x0F) + ((new_time.seconds / 16) * 10);
            new_time.minutes = (new_time.minutes & 0x0F) + ((new_time.minutes / 16) * 10);
            new_time.hours = ((new_time.hours & 0x0F) + (((new_time.hours & 0x70) / 16) * 10)) | (new_time.hours & 0x80);
            new_time.day = (new_time.day & 0x0F) + ((new_time.day / 16) * 10);
            new_time.month = (new_time.month & 0x0F) + ((new_time.month / 16) * 10);
            new_time.year = (new_time.year & 0x0F) + ((new_time.year / 16) * 10);
        }

        if (!(regB & 0x02) && (new_time.hours & 0x80)) {
            new_time.hours = ((new_time.hours & 0x7F) + 12) % 24;
        }
        size_t t = terminal_row;
        global_time = new_time;
        terminal_row = t;
    }
    get_rtc_reg(0x0C);
    PIC_sendEOI(8);
    asm volatile ("sti");

}


void enable_rtc(void) {
    uint8_t prev_regB = get_rtc_reg(0x0B);
    outb(CMOS_CONTROL, 0x8B);
    outb(CMOS_DATA, prev_regB | 0x40);
    get_rtc_reg(0x0C);
}

void print_time(void) {
    uint16_t yr = 2000 + global_time.year;
    printf("%d-%d-%dT%d:%d:%d\n", yr, global_time.month, global_time.day, global_time.hours, global_time.minutes, global_time.seconds);
}
