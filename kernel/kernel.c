#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "keyboard.h"
#include "../libc/string.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void init_terminal(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t * ) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void scroll_down() {
    for (size_t i = 0; i < VGA_HEIGHT - 1; i++) {
        for (size_t j = 0; j < VGA_WIDTH; j++) {
            const size_t characterBelowLocation = (i + 1) * VGA_WIDTH + j;
            char currentCharacter = terminal_buffer[characterBelowLocation];
            put_entry_at(currentCharacter, terminal_color, j, i);
        }
    }

    for (size_t i = 0; i < VGA_WIDTH; i++) {
        const size_t index = 24 * VGA_WIDTH + i;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }

    terminal_column = 0;
    terminal_row--;
}

void putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
    } else if(c == '\b'){

        if(terminal_column == 0 && terminal_row != 0){
            terminal_row--;
            terminal_column = VGA_WIDTH;
        }

        terminal_column--;
        putchar(' ');
        terminal_column--;
    }else {
        put_entry_at(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }

    if (terminal_row == VGA_HEIGHT) {
        scroll_down();
    }

    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}

void print(const char* data) {
    size_t size = strlen(data);
    for (size_t i = 0; i < size; i++) putchar(data[i]);
}

void itoa(uint8_t* buffer, uint32_t base, int32_t d) {
    uint8_t* p = buffer;
    uint8_t* p1;
    uint8_t* p2;
    uint32_t ud;
    uint32_t divisor = 10;

    if (base == 10 && d < 0) {
        *p++ = '-';
        ud = -d;
    } else {
        ud = (d < 0 && base == 10) ? -d : d;
        if (base == 16) {
            divisor = 16;
        }
    }

    do {
        uint32_t remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    *p = '\0';
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        uint8_t tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }
}

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

void kernel_main(void) {
    init_terminal();
    init_idt();
    init_gdt();
    init_keyboard();

    print("Hello, world!\n");
    for(;;);
}