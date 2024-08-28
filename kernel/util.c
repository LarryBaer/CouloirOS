#include <stdint.h>
#include "./include/util.h"

// Takes in a byte of data from specified port
char inPortB(uint16_t port){
    char data;
    asm volatile("inb %1, %0": "=a" (data) : "dN"(port));
    return data;
}

// Outputs a byte of data to specified idle port
void outPortB(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
