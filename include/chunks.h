#ifndef CHUNKS_H
#define CHUNKS_H

#include <stdint.h>



#define NUM_SUPPORTED_CHUNKS 4

void IHDR(uint8_t*, uint32_t);
void PLTE(uint8_t*, uint32_t);
void IDAT(uint8_t*, uint32_t);
void IEND(uint8_t*, uint32_t);



struct chunk_function {
    char name[5];
    void (*function)(uint8_t*, uint32_t);
};

extern const struct chunk_function chunk_functions[4];

#endif
