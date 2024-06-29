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
K_OFILES = $(patsubst %.c, %.o, $(K_CFILES))

K_ASMFILES = $(wildcard $(KERNELDIR)/*.asm)

LIBC_CFILES = $(wildcard $(LIBCDIR)/*.c)
LIBC_OFILES = $(patsubst %.c, %.o, $(LIBC_CFILES))

I_CFILES = $(wildcard $(INCLUDE_DIR)/*.c)
I_OFILES = $(patsubst %.c, %.o, $(I_CFILES))

BOOT_SRC = $(wildcard $(BOOTDIR)/*.asm)
BOOT_BIN = $(patsubst %.asm, %.bin, $(BOOT_SRC))
KERNEL_BIN = $(SRC)/stage2/kernel.bin

DISK_FILE = disk.img

.PHONY: all clean

all: $(DISK_FILE)

$(DISK_FILE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Generating disk.img file..."
	@dd if=/dev/zero of=$@ bs=512 count=100
	@dd if=./src/boot/boot.bin of=$@ conv=notrunc
	@dd if=$(KERNEL_BIN) of=$@ conv=notrunc seek=1

$(BOOTDIR)/%.bin: $(BOOTDIR)/%.asm
	@echo "ASM $<"
	@$(ASM) $(ASMFLAGS) -o $@ $<

$(KERNEL_BIN): $(K_OFILES) $(KERNELDIR)/loader.o $(LIBC_OFILES) $(I_OFILES)
	@echo "LD $<"
	@$(LD) -T $(SRC)/stage2/link.ld $(KERNELDIR)/loader.o $(K_OFILES) $(LIBC_OFILES) $(I_OFILES) -o $@ --oformat=binary 

$(LIBCDIR)/%.o: $(LIBCDIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNELDIR)/%.o: $(KERNELDIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(INCLUDE_DIR)/%.o: $(INCLUDE_DIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNELDIR)/loader.o: $(K_ASMFILES)
	@echo "ASM $<"
	@$(ASM) -f elf $< -o $@

clean:
	@echo "Clean up..."
	@rm -f $(BOOT_BIN) $(DISK_FILE) $(KERNEL_BIN) $(LIBC_OFILES) $(K_OFILES) $(I_OFILES) $(KERNELDIR)/loader.o

cleanup:
	@echo "Clean up object files..."
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(LIBC_OFILES) $(K_OFILES) $(I_OFILES) $(KERNELDIR)/loader.o

run: $(DISK_FILE)
	qemu-system-x86_64 -drive file=disk.img,format=raw

runb: $(DISK_FILE)
	bochs -f bochs_config_nodbg

run-debug: $(DISK_FILE)
	bochs -f bochs_config
