# Variables principales
LIB     = lib/libc.a
CC      = i686-elf-gcc
AR      = i686-elf-ar

LIBC_SRC = $(wildcard stdio/*.c) $(wildcard stdlib/*.c) $(wildcard string/*.c)
LIBC_OBJ = $(LIBC_SRC:%.c=%.o)

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_libc -Iincludes/ -I../kernel/includes/

# Création de la bibliothèque
$(LIB): $(LIBC_OBJ)
	@mkdir -p $(dir $(LIB))  # Assure-toi que le dossier libc/lib existe
	$(AR) rcs $@ $^

# Compilation des fichiers objets
stdio/%.o: stdio/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

stdlib/%.o: stdlib/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

string/%.o: string/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf lib
	rm -f stdio/*.o stdlib/*.o string/*.o $(LIB)
