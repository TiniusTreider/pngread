#ifndef CHUNKS_H
#define CHUNKS_H

#include "parser.h"

#include <stdbool.h>
#include <stdint.h>



struct chunk {
    char name[5];
    bool critical;
    bool supported;

    uint8_t *data;
    uint32_t length;
};

void print_chunk_header(struct chunk *chunk);

#define NUM_SUPPORTED_CHUNKS 4

void IHDR(uint8_t*, uint32_t);
void PLTE(uint8_t*, uint32_t);
void IDAT(uint8_t*, uint32_t);
void IEND(uint8_t*, uint32_t);

struct chunk_function {
    char name[5];
    void (*function)(uint8_t*, uint32_t);
};
extern const struct chunk_function chunk_functions[NUM_SUPPORTED_CHUNKS];

#endif
