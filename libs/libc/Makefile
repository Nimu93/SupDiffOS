LIB     = lib/libc.a
CC      = i686-elf-gcc
AR      = i686-elf-ar

LIBC_SRC = $(wildcard stdio/*.c) $(wildcard stdlib/*.c) $(wildcard string/*.c)
LIBC_OBJ = $(LIBC_SRC:%.c=%.o)

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_libc -Iincludes/ -I../kernel/includes/

$(LIB): $(LIBC_OBJ)
	@mkdir -p $(dir $(LIB)) 
	$(AR) rcs $@ $^

stdio/%.o: stdio/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

stdlib/%.o: stdlib/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

string/%.o: string/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf lib
	rm -f stdio/*.o stdlib/*.o string/*.o $(LIB)
