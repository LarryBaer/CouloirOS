#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include "../include/stdio.h"
#include "../include/string.h"
#include "../../kernel/include/terminal.h"

void printf(const char* format, ...) {
    uint8_t buffer[20];
    const char* str;
    int pad0 = 0, pad = 0;
    va_list args;

    va_start(args, format);

    while (*format) {
        char current = *format++;

        if (current != '%') {
            putchar(current);
            continue;
        }

        current = *format++;
        if (current == '0') {
            pad0 = 1;
            current = *format++;
        }

        if (current >= '0' && current <= '9') {
            pad = current - '0';
            current = *format++;
        }

        switch (current) {
            case 'X':
            case 'd':
            case 'u':
            case 'x':
                itoa(buffer, current, va_arg(args, int));
                str = buffer;
                break;
            case 's':
                str = va_arg(args, const char*);
                if (!str) {
                    str = "(null)";
                }
                break;
            default:
                putchar((char)va_arg(args, int)); 
                continue;
        }

        size_t length = strlen((const char*)str);
        size_t padding = (length < pad) ? pad - length : 0;

        for (size_t i = 0; i < padding; i++) {
            putchar(pad0 ? '0' : ' ');
        }

        while (*str) {
            putchar(*str++);
        }
    }

    va_end(args);
}