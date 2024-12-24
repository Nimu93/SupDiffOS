#include <stdio.h>
#include <stddef.h>

int puts(const char* string) {
	for (size_t i = 0; string[i] != '\0'; i++) {
		putchar(string[i]);
	}
	return 0;
}
