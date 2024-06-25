ASM = nasm
ASMFLAGS = -f bin
CC = i686-elf-gcc
CFLAGS = -ffreestanding -O2 -nostdlib -m32
LD = i686-elf-ld
LDFLAGS = -T src/C/link.ld -m elf_i386

SRC = ./src
BOOTDIR = $(SRC)/boot
STAGE2DIR = $(SRC)/stage2
CDIR = $(SRC)/C

BOOT_SRC = $(wildcard $(BOOTDIR)/*.asm)
STAGE2_SRC = $(wildcard $(STAGE2DIR)/*.asm)
BOOT_BIN = $(patsubst %.asm,%.bin,$(BOOT_SRC))
STAGE2_BIN = $(patsubst %.asm,%.bin,$(STAGE2_SRC))


DISK_FILE = disk.img

.PHONY: all clean

all: $(DISK_FILE)

$(DISK_FILE): $(BOOT_BIN) $(STAGE2_BIN)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOT_BIN) of=$@ conv=notrunc
	dd if=$(STAGE2_BIN) of=$@ conv=notrunc seek=1

$(BOOTDIR)/%.bin: $(BOOTDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(STAGE2DIR)/%.bin: $(STAGE2DIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -f $(BOOT_BIN) $(STAGE2_BIN) $(DISK_FILE)
