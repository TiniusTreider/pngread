#include "colors.h"
#include "error.h"
#include "io.h"
#include "parser.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static inline void top_data(void *data, size_t data_length) {
    const char NORMAL_SIGNATURE[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    throw_error_if(memcmp(NORMAL_SIGNATURE, data, 8) != 0, "Invalid PNG");

    printf(GREEN "\nValid PNG\n\n" RESET);
    printf(" - Size: %.2fkB\n", (float)data_length / 1000);
}



#define CHUNK_VECTOR_START_SIZE 8
#define CHUNK_VECTOR_GROW_SIZE  4

struct chunk {
    char name[5];
    bool critical;
    bool supported;

    uint32_t length;
    uint8_t *data;
};

struct chunk_vector {
    struct chunk *data;
    size_t count;
};

static inline void grow_chunk_vector(struct chunk_vector *vector) {
    vector->count += CHUNK_VECTOR_GROW_SIZE;
    vector->data = realloc(vector->data, vector->count);
    throw_error_if(vector->data == NULL, "Failed to allocate memory");
}

static inline void make_chunk_vector(void *data, size_t data_length, struct chunk_vector *chunks) {
    size_t chunk = 0;
    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;

    while (pointer < end_pointer) {
        const uint32_t length = read_big_endian_uint32(pointer, 0);

        char name_string[5];
        memcpy(name_string, pointer + 4, 4);
        throw_error_if(name_string[2] & 0b00100000, "Malformed chunk name");
        name_string[4] = '\0';

        if (chunk >= chunks->count)
            grow_chunk_vector(chunks);
        memcpy(chunks->data[chunk].name, name_string, 5);
        chunks->data[chunk].length = length;
        chunks->data[chunk].data   = pointer + 8;
        chunk++;

        const size_t offset = pointer - (uint8_t*)data;
        throw_error_if(offset > data_length - length - 12, "Invalid chunk size");
        pointer += length + 12;
    }
}



int compare_chunk_vectors(void *a, void *b) {
    // TODO: compare two (struct chunk)'s
}

static inline void run_chunk_functions(void *data, struct chunk_vector *chunks) {
    // TODO: sort chunks (critical, supported, unsupported)
    //
    // TODO: run function for each chunk if supported
}



void parse(char *path) {
    const size_t data_length = (size_t)file_length(path);
    void *data = safe_malloc(data_length);
    read_file(path, data_length, data);

    top_data(data, data_length);

    struct chunk_vector chunks = (struct chunk_vector){
        .data = safe_malloc(CHUNK_VECTOR_START_SIZE * sizeof(struct chunk)), CHUNK_VECTOR_START_SIZE
    };

    make_chunk_vector(data, data_length, &chunks);
    run_chunk_functions(data, &chunks);

    free(chunks.data);
    free(data);
    printf(GREEN "\nSuccess!\n" RESET);
}

