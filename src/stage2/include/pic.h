#pragma once
#include "./io.h"
#include "./libc/stdint.h"

// https://wiki.osdev.org/PIC

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2		    0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */

void PIC_sendEOI(uint8_t irq) {
	if(irq >= 8) outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}

// Disable PIC if using APIC or IOAPIC
void pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void IRQ_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
    if (irq < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

void IRQ_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
    if (irq < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void remap_PIC(void) {
    uint8_t pic_1_mask, pic_2_mask;
    pic_1_mask = inb(PIC1_DATA);
    pic_2_mask = inb(PIC2_DATA);

    // ICW 1
    outb(PIC1_COMMAND, 0x11);
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();
    // ICW 2
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();
    // ICW 3
    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();
    // ICW 4
    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, pic_1_mask);
    outb(PIC2_DATA, pic_2_mask);
}
