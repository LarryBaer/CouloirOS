#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "./include/idt.h"
#include "./include/terminal.h"
#include "./include/keyboard.h"
#include "./include/gdt.h"
#include "../lib/include/string.h"
#include "../lib/include/stdio.h"

void kernel_main() {
    init_terminal();
    init_idt();
    init_gdt();
    init_keyboard();
    printf("Hello, world!\n");
    for(;;);
}
