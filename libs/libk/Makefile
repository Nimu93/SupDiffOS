LIB     = lib/libk.a
CC      = i686-elf-gcc
AR      = i686-elf-ar

LIBC_SRC = $(wildcard ../libc/stdio/*.c) $(wildcard ../libc/stdlib/*.c) $(wildcard ../libc/string/*.c)
LIBC_OBJ = $(LIBC_SRC:%.c=%.o)

LIBC_BUILD_DIR = libc/lib

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_libk -Iincludes/ -I ../../kernel/includes/ -I../libc/includes/

$(LIB): $(LIBC_OBJ)
	@mkdir -p $(dir $(LIB))
	$(AR) rcs $@ $^

../libc/stdio/%.o: stdio/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

../libc/stdlib/%.o: stdlib/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

../libc/string/%.o: string/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf lib
	rm -f ../libc/stdio/*.o ../libc/stdlib/*.o ../libc/string/*.o $(LIB)
