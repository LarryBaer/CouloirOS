#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "./include/terminal.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

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