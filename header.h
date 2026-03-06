#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>
#include <stdint.h>

// main.c
void throw_error(char*);

// renderer.c
int render(char*);

//io.c
long file_length(char*);
void read_file(const char*, size_t, void*);

uint32_t read_big_endian_int32(void *data, size_t index);

#endif
