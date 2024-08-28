#ifndef UTIL_H
#define UTIL_H

typedef struct {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
} InterruptRegisters;

char inPortB(uint16_t port);
void outPortB(uint16_t port, uint8_t value);

#endif