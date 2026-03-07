#include "colors.h"
#include "parser.h"
#include "util.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const char NORMAL_SIGNATURE[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
static inline void signature(void *data) {
    if (memcmp(NORMAL_SIGNATURE, data, 8) != 0) {
        throw_error("Invalid PNG");
    } else {
        printf(GREEN "\nValid PNG\n\n" RESET);
    }
}

struct chunk {
    char name[5];
    uint32_t length;
    uint8_t *data;
};
static inline size_t count_chunks(void *data, size_t data_length) {
    size_t count = 0;

    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;
    while (pointer < end_pointer) {
        count++;

        const uint32_t length = read_big_endian_uint32(pointer, 0);
        if (pointer - (uint8_t*)data > data_length - length - 12) throw_error("Invalid chunk size");
        pointer += length + 12;
    }

    return count;
}
static inline void walk_chunks(void *data, size_t data_length, struct chunk *chunks) {
    size_t chunk = 0;
    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;
    while (pointer < end_pointer) {
        const uint32_t length = read_big_endian_uint32(pointer, 0);

        char name_string[5];
        memcpy(name_string, pointer + 4, 4);
        if (name_string[2] & 0b00100000) throw_error("Malformed chunk name");
        name_string[4] = '\0';

        memcpy(chunks[chunk].name, name_string, 5);
        chunks[chunk].length = length;
        chunks[chunk].data   = pointer + 8;
        chunk++;

        pointer += length + 12;
    }
}
int parse(char *path) {
    const size_t data_length = (size_t)file_length(path);
    void *data = malloc(data_length);
    if (data == NULL) throw_error("Failed to allocate memory");
    read_file(path, data_length, data);

    signature(data);
    printf(" - Size: %.2fkB\n", (float)data_length / 1000);

    const size_t count = count_chunks(data, data_length);

    struct chunk *chunks = malloc(count * sizeof(struct chunk));
    if (chunks == NULL) throw_error("Failed to allocate memory");
    walk_chunks(data, data_length, chunks);

    // WIP

    free(chunks);
    free(data);
    printf(GREEN "\nSuccess!\n" RESET);
    return 0;
}
