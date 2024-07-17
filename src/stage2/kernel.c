#include "drivers/serial.h"
#include "include/pit.h"
#include "include/rtc.h"
#include "krn.h"

__attribute__((interrupt)) void dbg(interrupt_frame *frame) {
    printf("Debug Interrupt called!\nPres any key to continue...\n");
    wait_key = 1;
    while (wait_key);
    asm ("sti");
}

uint16_t pit_freq = 0;
extern int ticks;

uint8_t x[8][8] = {
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,0,0,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
};

uint8_t heart[8][8] = {
    {0,1,1,0,0,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
};


void _kstart() {
    KINIT
    init_screen(0x00);
    int xx = 0, yy = 0;
    int c = 0x01;
    while (1) {
        msleep(1000);
        draw(heart, c, 0, 0);
        c = (c < 0xF7) ? c+1 : 0;
    }
}





/*CHANGE "mov al, 0x13" to mov al, 0x03 in boot.asm in order this to work*/
/* void _kstart() {
    KINIT
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    write_string_serial("Hello serial");
    // asm ("int $1"); // test
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
           continue;
        } else if (!strcmp(c, "ticks")) {
            printf("%d\n", ticks);
        } else {
            printf("Invalid command\n");
        }
    }
    // for(;;);
    halt;
} */

