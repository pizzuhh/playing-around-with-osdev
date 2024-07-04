#ifndef RTC_H
#define RTC_H
#include "./idt.h"
#include "./pic.h"
#include "./libc/stdbool.h"
#include "./libc/stdint.h"
#include "./libc/stdio.h"

#define CMOS_CONTROL 0x70
#define CMOS_DATA 0x71

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} __attribute__((packed)) datetime_t;

extern datetime_t global_time;

__attribute__((no_caller_saved_registers))
static bool is_update_in_progress(void) {
    outb(CMOS_CONTROL, 0x0A);
    return (inb(CMOS_DATA) & 0x8) != 0;
}
__attribute__((no_caller_saved_registers))
static uint8_t get_rtc_reg(uint16_t reg) {
    outb(CMOS_CONTROL, reg | 0x80);
    return inb(CMOS_DATA);
}

void enable_rtc(void);
void print_time(void);
__attribute__((interrupt)) void rtc_handler(interrupt_frame *frame);

#endif
