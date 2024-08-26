#include <stdint.h>
#include "util.h"

void memset(void* destination, char val, uint32_t count){
    char* temp = (char*) destination;
    for(; count != 0; count--){
        *temp++ = val;
    }
}

// Outputs a byte of data to specified idle port
void outPortB(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
