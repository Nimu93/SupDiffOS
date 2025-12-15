#include "keyboard.h"

static bool shift_pressed = false;

static char *ch_buffer;
static size_t ch_buffer_index;

void init_keyboard() {
    ch_buffer = (char*)kmalloc(KEYBOARD_BUFFER_SIZE);
    memset(ch_buffer, 0, KEYBOARD_BUFFER_SIZE);
    ch_buffer_index = 0;
}

char get_char() {
    if (ch_buffer_index == 0) {
        return 0; // Buffer is empty
    }
    char c = ch_buffer[0];
    // Shift buffer contents to the left
    memmove(ch_buffer, ch_buffer + 1, ch_buffer_index - 1);
    ch_buffer_index--;
    return c;
}

void fill_buffer(char c) {
    if (ch_buffer_index < KEYBOARD_BUFFER_SIZE - 1) {
        ch_buffer[ch_buffer_index++] = c;
    }
}
void get_keyboard(uint8_t scancode) {
     if (scancode & 0x80) {
        if (scancode == 0xAA || scancode == 0xB6)
        {
            shift_pressed = false;
        }
        goto end;
    }

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = true;
        goto end;
    }
    char c = scancode_to_ascii[scancode];

    if (c) {
        if (shift_pressed && c >= 'a' && c <= 'z')
            c -= 32; // majuscule
        fill_buffer(c);
    } else {
        printf("Unknown scancode: 0x%x\n", scancode);
    }
    // Send End-of-Interrupt (EOI) to PIC
    end:
    outb(0x20, 0x20);
}
