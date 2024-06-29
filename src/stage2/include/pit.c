#include "pit.h"

uint32_t sleep_counter = 0;
int ticks = 0;

__attribute__((interrupt)) void timer_irq_handler(interrupt_frame *frame) {
    if (sleep_counter > 0) (sleep_counter)--;
    PIC_sendEOI(0);
}

void sleep(uint32_t ms) {
    sleep_counter = ms;
    while (sleep_counter > 0) asm volatile ("nop");
    asm volatile ("sti");
    return;
}

/*The PIT runs at (roughly) 1.193182 MHz*/
void set_pit_mode_frequency(const uint8_t channel, const uint8_t mode, const uint16_t freq) {
    
    if (channel > 3 || mode > 7) return;
    asm volatile ("cli");
    outb(PIT_CHANNEL2, ((channel << 6) | (0b11 << 4) | (mode << 1)) );
    outb(PIT_CHANNEL0 + channel, (uint8_t)(freq)); // low
    outb(PIT_CHANNEL0 + channel, (uint8_t)(freq >> 8)); // high
    asm volatile ("sti");

}
