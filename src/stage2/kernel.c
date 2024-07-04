#define NULL ((void *)0)
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
#include "include/rtc.h"
uint16_t pit_freq = 0;
// https://wiki.osdev.org/Reboot#Short_code_to_do_a_8042_reset
void reboot(void)
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    halt;
}

__attribute__((interrupt)) void t(interrupt_frame *frame) {
    printf("Invalid opcode\n");
    frame->eip+=2;
}

void _kstart() {
    pit_freq = 1193;
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    set_idt_descriptor(4, overflow, TRAP_GATE);
    set_idt_descriptor(6, t, TRAP_GATE);
    pic_disable();
    remap_PIC();
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE); // PIT
    set_idt_descriptor(0x21, kbd_handler, INTERRUPT_GATE);
    set_idt_descriptor(0x28, rtc_handler, INTERRUPT_GATE);
    IRQ_clear_mask(0);
    IRQ_clear_mask(1);
    IRQ_clear_mask(2); // PIC2
    IRQ_clear_mask(8); // RTC
    asm volatile ("sti");
    enable_rtc();
    set_pit_mode_frequency(0, MODE2, pit_freq);    
    printf("Welcome to the kernel of this\nRun `help` for a list of commands\n");
    for(;;) {
        printf("> ");
        char *c = get_input();
        if (!strcmp(c, "beep")) {
            int bpm = 660;
            terminal_clear(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));
            msleep(70);
            terminal_clear(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
            msleep(70);
            terminal_clear(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));
            msleep(70);
            terminal_clear(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
            printd(100, "You are gonna have a bad time!");
            printf("\n\nSANS\n\n");
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
        } else if (!strcmp(c, "time")){
            print_time();

        } else if (!strcmp(c, "dog")) {
            
            printf("CAT\n");
            uint16_t d[256*1];
            ata_read_sector(0, 1, d);
            ata_write_sector(1, 1, d);
            reboot();
        } else if (!strcmp(c, "help")) {
            printf("beep - play beep\ndog - prints cat(tests ata_write_sector() which erases the kernel)\nhelp - prints this menu\ntime - displays the current RTC time. The timezone is (probably) UTC\nreboot - reboots the system\nshutdown - shutdowns the system (qemu only)\n");
        } else if (!strcmp(c, "shutdown")) {
            outw(0x604, 0x2000);
        } else if (!strcmp(c, "reboot")) {
            reboot();
        } else if (!strcmp(c, "test")) {
           asm volatile ("ud2");
           int c = 10/0;
           //continue;
        } else {
            printf("Invalid command\n");
        }
    }
    
    // for(;;);
    halt;
}
