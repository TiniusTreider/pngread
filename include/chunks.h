#ifndef CHUNKS_H
#define CHUNKS_H

#include <stdint.h>



void IHDR(uint8_t*, uint32_t);
void PLTE(uint8_t*, uint32_t);
void IDAT(uint8_t*, uint32_t);
void IEND(uint8_t*, uint32_t);

#endif
