# Variables
CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
GRUB_MKRESCUE = grub-mkrescue
QEMU = qemu-system-i386

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I kernel/includes/
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

# Répertoires et fichiers
KERNEL_SRC_DIR = kernel
KERNEL_SRC = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJ = $(KERNEL_SRC:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.o)
BOOT_SRC = boot.s
BOOT_OBJ = boot.o
KERNEL_BIN = myos.bin
ISO_DIR = isodir
ISO_FILE = myos.iso
GRUB_CFG = grub.cfg

# Cible par défaut
all: $(ISO_FILE)

# Compilation du bootloader
$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $< -o $@

# Compilation des fichiers sources du noyau
$(KERNEL_SRC_DIR)/%.o: $(KERNEL_SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Création de l'exécutable final
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc

# Création de l'ISO
$(ISO_FILE): $(KERNEL_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/$(KERNEL_BIN)
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

# Nettoyage des fichiers générés
clean:
	rm -f $(BOOT_OBJ) $(KERNEL_OBJ) $(KERNEL_BIN)
	rm -rf $(ISO_DIR) $(ISO_FILE)

# Lancement avec QEMU (ISO)
run-iso: $(ISO_FILE)
	$(QEMU) -cdrom $<

# Lancement avec QEMU (directement le kernel)
run-kernel: $(KERNEL_BIN)
	$(QEMU) -kernel $<

# Cibles utilitaires
.PHONY: all clean run-iso run-kernel