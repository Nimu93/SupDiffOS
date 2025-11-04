#include "keyboard.h"

static bool shift_pressed = false;

void get_keyboard(uint8_t scancode) {
     if (scancode & 0x80) {
        if (scancode == 0xAA || scancode == 0xB6)
        {
            shift_pressed = false;
        }
        goto end;
    }

    if (scancode == 0x2A || scancode == 0x36) {
        printf("Shift key pressed\n");
        shift_pressed = true;
        goto end;
    }
    char c = scancode_to_ascii[scancode];

    if (c) {
        if (shift_pressed && c >= 'a' && c <= 'z')
            c -= 32; // majuscule

        if (c == '\n')
            printf("Key pressed: Enter\n");
        else if (c == '\b')
            printf("Key pressed: Backspace\n");
        else if (c == '\t')
            printf("Key pressed: Tab\n");
        else if (c == ' ')
            printf("Key pressed: Space\n");
        else
            printf("Key pressed: %c\n", c);
    } else {
        printf("Unknown scancode: 0x%x\n", scancode);
    }
    // Send End-of-Interrupt (EOI) to PIC
    end:
    outb(0x20, 0x20);
}