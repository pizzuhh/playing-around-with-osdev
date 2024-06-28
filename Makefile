ASM = nasm
ASMFLAGS = -f bin
CC = i686-elf-gcc
CFLAGS = -ffreestanding -nostdlib -m32 -g -mgeneral-regs-only
LD = i686-elf-ld

SRC = ./src
BOOTDIR = $(SRC)/boot
KERNELDIR = $(SRC)/stage2
INCLUDE_DIR = $(KERNELDIR)/include
LIBCDIR = $(INCLUDE_DIR)/libc

K_CFILES = $(wildcard $(KERNELDIR)/*.c)
K_ASMFILES = $(wildcard $(KERNELDIR)/*.asm)

LIBC_CFILES = $(wildcard $(LIBCDIR)/*.c)
LIBC_OFILES = $(patsubst %.c, %.o, $(LIBC_CFILES))

K_COFILES += $(patsubst %.c, %.o, $(K_CFILES))

BOOT_SRC = $(wildcard $(BOOTDIR)/*.asm)
BOOT_BIN = $(patsubst %.asm,%.bin,$(BOOT_SRC))
KERNEL_BIN = $(SRC)/stage2/kernel.bin

DISK_FILE = disk.img

.PHONY: all clean

all: $(DISK_FILE)

$(DISK_FILE): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=512 count=100
	dd if=$(BOOT_BIN) of=$@ conv=notrunc
	dd if=$(KERNEL_BIN) of=$@ conv=notrunc seek=1

$(BOOTDIR)/%.bin: $(BOOTDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(KERNEL_BIN): $(K_COFILES) $(KERNELDIR)/loader.o $(LIBC_OFILES)
	$(LD) -T $(SRC)/stage2/link.ld $(SRC)/stage2/loader.o $(K_COFILES) $(LIBC_OFILES) -o $@ --oformat=binary 

$(LIBCDIR)/%.o: $(LIBCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(K_COFILES): $(K_CFILES)
	$(CC) $(CFLAGS) -c $< -o $@



$(KERNELDIR)/loader.o: $(K_ASMFILES)
	$(ASM) -f elf $< -o $@


clean:
	rm -f $(BOOT_BIN) $(STAGE2_BIN) $(DISK_FILE) $(KERNEL_BIN)

run: $(DISK_FILE)
	qemu-system-x86_64 -drive file=disk.img,format=raw
run-debug: $(DISK_FILE)
	bochs -f bochs_config
