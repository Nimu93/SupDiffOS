CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

KERNEL_SRC = kernel.c init_k.c terminal.c
KERNEL_OBJ = $(KERNEL_SRC:%.c=%.o)
KERNEL_BIN = supdiff.bin

LIBC_PATH = ../libs/libc/lib
LIBC = $(LIBC_PATH)/libc.a 

LIBK_PATH = ../libs/libk/lib
LIBK = $(LIBK_PATH)/libk.a 


LINKER=../linker.ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_kernel -I includes/ -I ../libs/libc/includes/
LDFLAGS = -T $(LINKER) -ffreestanding -O2 -nostdlib -lgcc -L ../libs/libc/lib -lc

BOOT_SRC = ../boot.s
BOOT_OBJ = boot.o

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(LIBK)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(BOOT_OBJ) $(KERNEL_OBJ) $(KERNEL_BIN)
	rm -rf $(ISO_DIR) $(ISO_FILE)