#include "include/graphics.h"
#include "krn.h"

#define NULL (void*)0

uint8_t graphics_mode = 0;
__attribute__((interrupt)) void dbg(interrupt_frame *frame) {
    prints("Debug Interrupt called!\nPres any key to continue...\n");
    printk("Debug Interrupt called!\nPres any key to continue...\n");
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


void print_memory(bool print_detected) {
    uint8_t tmp = default_color[1];
    default_color[1] = 0x0C;
    uint32_t num_entries = *(uint32_t*)0x8000;
    SMAP_mem *mem = (SMAP_mem*)0x8004;
    if (print_detected) {
        for (uint32_t i = 0; i < num_entries; i++) {
            printk("Region: 0x%08x ", mem->base_address);
            printk("Size: 0x%08x ", mem->len);
            switch (mem->type) {
                case 1:
                    printk("(A)\n");
                    break;
                default:
                case 2:
                    printk("(R)\n");
                    break;
                case 3:
                    printk("(AR)\n");
                    break;
                case 4:
                    printk("(ANM)\n");
                    break;
            }
            mem++;
        }
    } else mem += num_entries;
    mem--;
    
    printk("Total Memory: 0x%08x\n", (mem->base_address + mem->len - 1));
    printk("Used Memory:  0x%08x\n", used_blocks);
    printk("Free Memory:  0x%08x\n", max_blocks - used_blocks);
    default_color[1] = tmp;
}

void aaa(int a, char c) {
    for (int i = a; i > 0; i--) {
        for(int j = i; j > 0; j--) {
            printk("%c", c);
        }
        printk("\n");
    }
}

#if 1
void _kstart() {
    graphics_mode = 1;
    KINIT
    init_screen(0x00, 0x0F);
    finish();
    printk("Commands: 'print memory'\nPAGE UP to scoll up\nPAGE DOWN to scroll down\n\n");
    for(;;) {
        printk("> ");
        char *in = get_input();
        if (!strcmp(in, "print memory")) {
            print_memory(1);
            printk("TEST\n");
            char *str = alloc_blocks(1);
            strcpy(str, "Test string");
            printk("str: %s\nstr_addr: %08x\n", str, (uint32_t)str);
            print_memory(0);
            free_blocks(str, 3);
            printk("Freed\n");
            print_memory(0);
        } else if (!strcmp(in, "test")) {
            aaa(24, 'x');
        } else if (!strcmp(in, "draw")) { // Read 125 sectros from LBA 69 
            uint16_t *picture = alloc_blocks(16);
            // uint16_t data[2] = {0x0F00, 0x0F0C};
            // memcpy(picture, data, sizeof(data));
            // ata_write_sector(69, 5, picture);
            // memcpy(picture, 0x00, 5*4096); testing code
            ata_read_sector(69, 125, picture);
            uint16_t *original_picture = picture;
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; x += 2) { // += since uint16_t contains 2 pixels 
                    putpixel(x, y, *(picture) >> 8); // higher 8 bits
                    putpixel(x + 1, y, *(picture) & 0xFF); // lower 8 bits
                    picture++;
                }
            }
            free_blocks(original_picture, 16);
            picture = NULL;
        }
    }
}
#endif



#if 0
/*CHANGE "mov al, 0x13" to mov al, 0x03 in boot.asm in order this to work*/
void _kstart() {
    graphics_mode = 0;
    KINIT
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    write_string_serial("Hello serial ");
    print_memory();
    for(;;) {
        int i = 0;
        printk("> ");
        char *c = get_input(&i);
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
            printk("\n\nSANS\n\n");
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
            print_time(0);
        } else if (!strcmp(c, "dog")) {
            
            printk("CAT\n");
            uint16_t d[256*1];
            ata_read_sector(0, 1, d);
            ata_write_sector(1, 1, d);
            reboot();
        } else if (!strcmp(c, "help")) {
            printk("beep - play beep\ndog - prints cat(tests ata_write_sector() which erases the kernel)\nhelp - prints this menu\ntime - displays the current RTC time. The timezone is (probably) UTC\nreboot - reboots the system\nshutdown - shutdowns the system (qemu only)\n");
        } else if (!strcmp(c, "shutdown")) {
            outw(0x604, 0x2000);
        } else if (!strcmp(c, "reboot")) {
            reboot();
        } else if (!strcmp(c, "test")) {
           asm volatile ("ud2");
           continue;
        } else if (!strcmp(c, "ticks")) {
            printk("%d\n", ticks);
        } else {
            printk("Invalid command\n");
        }
    }
    // for(;;);
    halt;
}

#endif
