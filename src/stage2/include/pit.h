#ifndef PIT_H
#define PIT_H
#include "./pic.h"
#include "./io.h"
#include "./idt.h"
#include "libc/stdint.h"

extern uint16_t pit_freq;

extern uint32_t sleep_counter;

#define PIT_CHANNEL0    0x40 // IRQ0 clock stuff
#define PIT_CHANNEL1    0x41 // unused https://wiki.osdev.org/Programmable_Interval_Timer#Channel_1
#define PIT_CHANNEL2    0x42 // PC speaker
#define PIT_CMD_REG     0x43 // PIT command I/O port. Used in set_pit_mode_frequency()

extern int ticks;

__attribute__((interrupt)) void timer_irq_handler(interrupt_frame *frame);

void msleep(uint32_t ms);

// PIT modes
#define MODE0 0b000 // interrupt on terminal count
#define MODE1 0b001 // hardware re-triggerable one-shot
#define MODE2 0b010 // rate generator
#define MODE3 0b011 // square wave generator
#define MODE4 0b100 // software triggered strobe
#define MODE5 0b101 // hardware triggered strobe

/*The PIT runs at (roughly) 1.193182 MHz*/
void set_pit_mode_frequency(const uint8_t channel, const uint8_t mode, const uint16_t freq);

/* PC speaker */
void StartSound(unsigned int Frequency);
void StopSound();
void beep(unsigned int Frequency, unsigned int Duration);
#endif
