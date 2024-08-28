#include <stddef.h>
#include <stdint.h>

#ifndef STRING_H
#define STRING_H

size_t strlen(const char* str);
void memset(void* destination, char val, uint32_t count);

#endif