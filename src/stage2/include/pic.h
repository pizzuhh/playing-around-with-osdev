#ifndef PIC_H
#define PIC_H
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

void PIC_sendEOI(uint8_t irq);

// Disable PIC if using APIC or IOAPIC
void pic_disable(void);

void IRQ_set_mask(uint8_t IRQline);

void IRQ_clear_mask(uint8_t IRQline);
void remap_PIC(void);
#endif
