#include <stdint.h>
#include "../include/stdio.h"

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
