#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <stdint.h>



long file_length(char*);
void read_file(const char*, size_t, void*);

uint32_t read_big_endian_uint32(void *data, size_t index);

#endif
