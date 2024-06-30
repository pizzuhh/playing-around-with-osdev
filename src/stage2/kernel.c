#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/idt.h"
#include "include/exceptions.h"
#include "include/pic.h"
#include "include/pit.h"

uint16_t pit_freq = 0;

void StartSound(unsigned int Frequency)
{
	unsigned int Divisor = 1193180 / Frequency;

	/* outb(0x43, 0xB6);
	outb(0x42, Divisor & 0xFF);
	outb(0x42, Divisor >> 8); */
    set_pit_mode_frequency(2, MODE3, Divisor);

	outb(0x61, inb(0x61) | 3);
}

void StopSound()
{
	outb(0x61, inb(0x61) & ~3);
}

void beep(unsigned int Frequency, unsigned int Duration)
{
	StartSound(Frequency);
	msleep(Duration);
	StopSound();
}

void _kstart() {
    pit_freq = 1193;
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    pic_disable();
    remap_PIC();
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE);
    IRQ_clear_mask(0);
    asm volatile ("sti");
    set_pit_mode_frequency(0, MODE2, pit_freq);

    for (;;)  {
        beep(391, 100);
        beep(400, 100);
        beep(200, 100);
        beep(500, 100);
        beep(300, 100);
        beep(100, 100);
        beep(512, 100);
    }
    for(;;);
    halt;
}
