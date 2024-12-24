#include "init_k.h"

#define WIDTH 80  
#define HEIGHT 22

int init_drivers(void)
{
    // Initialize the terminal
    terminal_initialize();
    return 0;
}

void print_welcome_message(void) {
    const char* message = "SUP DIFF";

    size_t message_len = 8; 
    size_t start_pos = (WIDTH - message_len) / 2;

    printf("------------------------------------------------------------------------------\n");

    for (int i = 0; i < (HEIGHT / 2) - 1; i++) {
        printf("\n");
    }

    printf("%*s%s%*s\n", (int)start_pos, "", message, (int)(WIDTH - start_pos - message_len), "");

    for (int i = 0; i < (HEIGHT / 2) - 1; i++) {
        printf("\n");
    }
    printf("------------------------------------------------------------------------------\n");
}
