#include "drivers/ata.h"
#include "include/io.h"
#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/libc/string.h"
#include "include/idt.h"
#include "include/exceptions.h"
#include "include/pic.h"
#include "include/pit.h"
#include "drivers/keyboard.h"


uint16_t pit_freq = 0;

void _kstart() {
    pit_freq = 1193;
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    pic_disable();
    remap_PIC();
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE); // PIT
    set_idt_descriptor(0x21, kbd_handler, INTERRUPT_GATE); // TODO: implement the driver for: PS/2 keyboard 
    IRQ_clear_mask(0);
    IRQ_clear_mask(1);
    asm volatile ("sti");
    set_pit_mode_frequency(0, MODE2, pit_freq);    
    printf("Kernel\nRun `help` for a list of commands\n");
    for(;;) {
        printf("> ");
        char *c = get_input();
        if (!strcmp(c, "beep")) {
            int bpm = 660;
            printd(100, "You are gonna have a bad time!");
            for (;;) {
                // https://github.com/Passeriform/Mega-beep-ania/blob/master/megalovania.c
                // I'll probably make my own version :)
                // enjoy this for now
            beep(330, bpm/8);
            beep(330, bpm/8);
            beep(659, bpm/4);
            beep(494, bpm/2);
            beep(466, bpm/4);
            beep(440, bpm/4);
            beep(392, bpm/4);
            beep(330, bpm/8);
            beep(392, bpm/8);
            beep(440, bpm/8);
        
            beep(294, bpm/8);
            beep(294, bpm/8);
            beep(659, bpm/4);
            beep(494, bpm/2);
            beep(466, bpm/4);
            beep(440, bpm/4);
            beep(392, bpm/4);
            beep(330, bpm/8);
            beep(392, bpm/8);
            beep(440, bpm/8);
        
            beep(262, bpm/8);
            beep(262, bpm/8);
            beep(659, bpm/4);
            beep(494, bpm/2);
            beep(466, bpm/4);
            beep(440, bpm/4);
            beep(392, bpm/4);
            beep(330, bpm/8);
            beep(392, bpm/8);
            beep(440, bpm/8);
        
            beep(294, bpm/8);
            beep(294, bpm/8);
            beep(659, bpm/4);
            beep(494, bpm/2);
            beep(466, bpm/4);
            beep(440, bpm/4);
            beep(392, bpm/4);
            beep(330, bpm/8);
            beep(392, bpm/8);
            beep(440, bpm/8);
        }

        } else if (!strcmp(c, "dog")) {
            printf("CAT\n");
            uint16_t d[256*40];
            for (uint32_t i = 0; i < 256*40; i++) {
                d[i] = 0xFFFF;
            }
            ata_write_sector(1, 3, d);
            ata_write_sector(4, 2, d);
        } else if (!strcmp(c, "help")) {
            printf("beep - play beep\ndog - prints cat(tests ata_write_sector() which erases the kernel)\nhelp - prints this menu\n");
        }
    }
    
    
    halt;
}
