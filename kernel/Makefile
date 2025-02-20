CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
NASM = nasm

KERNEL_SRC = kernel.c init_k.c terminal.c memory/memory.c isr/isr.c gdt/gdt.c isr/idt.c
KERNEL_OBJ = $(KERNEL_SRC:%.c=%.o)
KERNEL_BIN = supdiff.bin

KERNEL_ASM_SRC = isr/idt_interrupts.asm memory/enable_paging.asm
KERNEL_ASM_OBJ = $(KERNEL_ASM_SRC:%.asm=%.o)  # ✅ Correction ici

LIBC_PATH = ../libs/libc/lib
LIBC = $(LIBC_PATH)/libc.a 

LIBK_PATH = ../libs/libk/lib
LIBK = $(LIBK_PATH)/libk.a 

LINKER=../linker.ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -g
CPPFLAGS = -D__is_kernel -I includes/ -I ../libs/libc/includes/
LDFLAGS = -T $(LINKER) -ffreestanding -O2 -nostdlib -lgcc -L ../libs/libc/lib -lc

BOOT_SRC = ../boot.s 
BOOT_OBJ = boot.o

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(LIBK) $(LIBC) $(KERNEL_ASM_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $< -o $@

%.o: %.asm  # ✅ Nouvelle règle pour assembler chaque fichier séparément
	$(NASM) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(BOOT_OBJ) $(KERNEL_OBJ) $(KERNEL_BIN) $(KERNEL_ASM_OBJ)
	rm -rf $(ISO_DIR) $(ISO_FILE)
