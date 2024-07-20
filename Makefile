ASM = nasm
ASMFLAGS = -f bin
CC = i686-elf-gcc
CFLAGS = -ffreestanding -nostdlib -m32 -mgeneral-regs-only -I./src/stage2/include -g
LD = i686-elf-ld
LDFLAGS = -T $(SRC)/stage2/link.ld $(KERNELDIR)/loader.o -Map map.map

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

M_CFILES = $(wildcard $(INCLUDE_DIR)/memory/*.c)
M_OFILES = $(patsubst %.c, %.o, $(M_CFILES))

DRIVER_DIR = $(SRC)/stage2/drivers
D_CFILES = $(wildcard $(DRIVER_DIR)/*.c)
D_OFILES = $(patsubst %.c, %.o, $(D_CFILES))

BOOT_SRC = $(wildcard $(BOOTDIR)/*.asm)
BOOT_BIN = $(patsubst %.asm, %.bin, $(BOOT_SRC))
KERNEL_BIN = $(SRC)/stage2/kernel.bin
KERNEL_ELF = $(SRC)/stage2/kernel.elf

DISK_FILE = disk.img

.PHONY: all clean

all: $(DISK_FILE)

$(DISK_FILE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Generating disk.img file..."
	@dd if=/dev/zero of=$@ bs=512 count=2880
	@dd if=./src/boot/boot.bin of=$@ bs=512 conv=notrunc
	@dd if=$(KERNEL_BIN) of=$@ bs=512 conv=notrunc seek=1
	@size=$$(wc -c < $(KERNEL_BIN));\
	SECTORS=$$(((size + 511) / 512));\
	echo "$@" "0x$$(printf '%X' $$SECTORS)"

$(BOOTDIR)/%.bin: $(BOOTDIR)/%.asm
	@echo "ASM $<"
	@$(ASM) $(ASMFLAGS) -o $@ $<

$(KERNEL_BIN): $(K_OFILES) $(KERNELDIR)/loader.o $(LIBC_OFILES) $(I_OFILES) $(D_OFILES) $(M_OFILES)
	@echo "LD $<"
	@$(LD) $(LDFLAGS) $(K_OFILES) $(LIBC_OFILES) $(I_OFILES) $(D_OFILES) $(M_OFILES) -o $(KERNEL_ELF) 
	i686-elf-objcopy -O binary $(KERNEL_ELF) $@

$(LIBCDIR)/%.o: $(LIBCDIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNELDIR)/%.o: $(KERNELDIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(INCLUDE_DIR)/%.o: $(INCLUDE_DIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(INCLUDE_DIR)/memory/%.o: $(INCLUDE_DIR)/memory/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	
$(KERNELDIR)/loader.o: $(K_ASMFILES)
	@echo "ASM $<"
	@$(ASM) -f elf $< -o $@

$(DRIVER_DIR)/%.o: $(DRIVER_DIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Clean up..."
	@rm -f $(BOOT_BIN) $(DISK_FILE) $(KERNEL_BIN) $(LIBC_OFILES) $(K_OFILES) $(I_OFILES) $(KERNELDIR)/loader.o serial.log

cleanup:
	@echo "Clean up object files..."
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(LIBC_OFILES) $(K_OFILES) $(I_OFILES) $(KERNELDIR)/loader.o

run: $(DISK_FILE)
	qemu-system-i386 -drive file=disk.img,format=raw -audiodev pa,id=speaker -machine pcspk-audiodev=speaker -serial file:serial.log

runb: $(DISK_FILE)
	bochs -f bochsrc

debug: $(DISK_FILE)
	qemu-system-i386 -drive file=disk.img,format=raw -audiodev pa,id=speaker -machine pcspk-audiodev=speaker -S -gdb tcp::1234
