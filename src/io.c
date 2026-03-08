#include "error.h"
#include "io.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>



void read_file(const char *path, size_t *length_buffer, void **data_buffer_pointer) {
    FILE *file = fopen(path, "rb");
    throw_error_if(file == NULL, "Could not open file");

    throw_error_if(fseek(file, 0, SEEK_END) != 0, "Could not navigate file");

    const long length = ftell(file);
    throw_error_if(length <= 0, "Could not navigate file");
    *length_buffer = length;

    void *data_buffer = safe_malloc(length);
    *data_buffer_pointer = data_buffer;

    throw_error_if(fseek(file, 0, SEEK_SET) != 0, "Could not navigate file");

    size_t elements = fread(data_buffer, 1, length, file);
    throw_error_if(elements != length, "Could not read file");

    throw_error_if(fclose(file) == EOF, "Could not close file");
}



#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    uint32_t read_big_endian_uint32(void *data, size_t index) {
        const uint8_t *little_endian = (uint8_t*)data + index;
        return (
            ((uint32_t)little_endian[0] << 24) |
            ((uint32_t)little_endian[1] << 16) |
            ((uint32_t)little_endian[2] << 8) |
            ((uint32_t)little_endian[3])
        );
    }

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

    uint32_t read_big_endian_uint32(void *data, size_t index) {
        return ((uint32_t*)data)[index];
    }

#else

    #error "Unsupported endianness"

#endif

