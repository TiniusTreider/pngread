#include "chunks.h"
#include "colors.h"
#include "error.h"
#include "io.h"
#include "parser.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static inline void print_signature(void *data, size_t data_length)
{
    const char NORMAL_SIGNATURE[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    throw_error_if(memcmp(NORMAL_SIGNATURE, data, 8) != 0, "Invalid PNG");

    printf(
        GREEN "\nValid PNG\n" RESET
        " - Size: .......... %.2fkB\n",
        (float)data_length / 1000
    );
}



#define CHUNK_VECTOR_START_SIZE 8
#define CHUNK_VECTOR_GROW_SIZE  4

struct chunk_vector
{
    struct chunk *data;
    size_t capacity;
    size_t count;
};

static inline void grow_chunk_vector(struct chunk_vector *vector)
{
    vector->capacity += CHUNK_VECTOR_GROW_SIZE;
    vector->data = realloc(vector->data, vector->capacity * sizeof(struct chunk));
    throw_error_if(vector->data == NULL, "Failed to allocate memory");
}

#define CRITICAL_BIT 0x20

static inline int32_t name_to_int(const char *name)
{
    return (
        (int32_t)name[0] |
        (int32_t)name[1] << 8 |
        (int32_t)name[2] << 16 |
        (int32_t)name[3] << 24
    );
}

static inline void make_chunk_vector(void *data, size_t data_length, struct chunk_vector *chunks)
{
    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;

    for (size_t chunk = 0; pointer < end_pointer; chunk++)
    {
        const size_t end_offset   = end_pointer - pointer;
        throw_error_if(end_offset < 12, "Truncated chunk header");

        const uint32_t length = read_big_endian_uint32(pointer, 0);
        const size_t start_offset = pointer - (uint8_t*)data;
        throw_error_if(start_offset + length + 12 > data_length, "Invalid chunk size");

        char name_string[5];
        memcpy(name_string, pointer + 4, 4);
        throw_error_if(name_string[2] & CRITICAL_BIT, "Invalid chunk name");
        name_string[4] = '\0';

        if (chunk >= chunks->capacity)
            grow_chunk_vector(chunks);

        struct chunk *this_chunk = chunks->data + chunk;

        memcpy(this_chunk->name, name_string, 5);

        this_chunk->critical = ~*name_string & CRITICAL_BIT;
        this_chunk->supported = false;
        int32_t chunk_name_int = name_to_int(name_string);
        for (size_t index = 0; index < NUM_SUPPORTED_CHUNKS; index++)
        {
            if (chunk_name_int == name_to_int(chunk_functions[index].name))
                this_chunk->supported = true;
        }

        this_chunk->length = length;
        this_chunk->data   = pointer + 8;

        chunks->count++;

        pointer += length + 12;
    }
}



int compare_chunks(const void *a, const void *b)
{
    const struct chunk *chunk_a = (const struct chunk*)a;
    const struct chunk *chunk_b = (const struct chunk*)b;

    int value_a = chunk_a->critical * 2 + chunk_a->supported;
    int value_b = chunk_b->critical * 2 + chunk_b->supported;

    return value_b - value_a;
}

static inline void run_chunk_functions(void *data, struct chunk_vector *chunks)
{
    qsort(chunks->data, chunks->count, sizeof(struct chunk), compare_chunks);

    for (size_t chunk = 0; chunk < chunks->count; chunk++)
    {
        struct chunk *this_chunk = chunks->data + chunk;

        if (!this_chunk->critical)
            printf(YELLOW);
        if (!this_chunk->supported)
            printf(RED);

        print_chunk_header(this_chunk);

        for (size_t index = 0; index < NUM_SUPPORTED_CHUNKS; index++)
        {
            if (memcmp(this_chunk->name, chunk_functions[index].name, 4) == 0)
                chunk_functions[index].function(this_chunk->data, this_chunk->length);
        }

        printf(RESET);
    }
}



void parse(char *path)
{
    size_t data_length;
    void *data;
    read_file(path, &data_length, &data);
    throw_error_if(data_length < 8, "Truncated file signature");

    print_signature(data, data_length);

    struct chunk_vector chunks = (struct chunk_vector)
    {
        .data = safe_malloc(CHUNK_VECTOR_START_SIZE * sizeof(struct chunk)),
        .capacity = CHUNK_VECTOR_START_SIZE,
        .count = 0
    };

    make_chunk_vector(data, data_length, &chunks);
    run_chunk_functions(data, &chunks);

    free(chunks.data);
    free(data);
    printf(GREEN "\nSuccess!\n" RESET);
}

