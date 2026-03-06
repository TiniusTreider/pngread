#include "header.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>



long file_length(char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) throw_error("Could not open file");

    if (fseek(file, 0, SEEK_END) != 0) throw_error("Could not navigate file");

    const long length = ftell(file);
    if (length == -1L) throw_error("Could not navigate file");

    if (fclose(file) == EOF) throw_error("Could not close file");

    return length;
}

void read_file(const char *path, size_t length, void *buffer) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) throw_error("Could not open file");

    if (fseek(file, 0, SEEK_END) != 0) throw_error("Could not navigate file");
    long file_length = ftell(file);
    if (file_length == -1L) throw_error("Could not navigate file");
    if (file_length < length) throw_error("Attempted to read past file");

    if (fseek(file, 0, SEEK_SET) != 0) throw_error("Could not navigate file");
    size_t elements = fread(buffer, 1, length, file);
    if (elements != length) throw_error("Could not read file");

    if (fclose(file) == EOF) throw_error("Could not close file");
}

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint32_t read_big_endian_int32(void *data, size_t index) {
        const uint8_t *little_endian = (uint8_t*)data + index;
        return (
            ((uint32_t)little_endian[0] << 24) |
            ((uint32_t)little_endian[1] << 16) |
            ((uint32_t)little_endian[2] << 8) |
            ((uint32_t)little_endian[3])
        );
    }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint32_t read_big_endian_int32(void *data, size_t index) {
        return ((uint32_t*)data)[index];
    }
#else
    #error "Unsupported endianness"
#endif
