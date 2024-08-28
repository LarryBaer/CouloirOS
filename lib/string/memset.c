#include <stdint.h>

void memset(void* destination, char val, uint32_t count){
    char* temp = (char*) destination;
    for(; count != 0; count--){
        *temp++ = val;
    }
}