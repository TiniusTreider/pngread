#include "header.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED    "\033[38;5;9m"
#define GREEN  "\033[38;5;10m"
#define YELLOW "\033[38;5;11m"
#define RESET  "\033[0m"

void * data;


const char NORMAL_SIGNATURE[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
static inline void signature(void) {
    if (memcmp(NORMAL_SIGNATURE, data, 8) != 0) {
        throw_error("Invalid PNG");
    } else {
        printf(GREEN "\nValid PNG\n\n" RESET);
    }
}

static inline void IHDR(void) {
    //WIP
}

static inline void PLTE(void) {
    //WIP
}

static inline void IDAT(void) {
    //WIP
}

static inline void IEND(void) {
    //WIP
}

int render(char *path) {
    const size_t data_length = (size_t)file_length(path);
    data = malloc(data_length);
    if (data == NULL) throw_error("Failed to allocate memory");
    read_file(path, data_length, data);

    signature();
    printf("Size: %.2fkB\n\n", (float)data_length / 1000);

    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;
    while (pointer < end_pointer) {
        const uint32_t length = read_big_endian_int32(pointer, 0);

        char name_string[5];
        memcpy(name_string, pointer + 4, 4);
        name_string[4] = '\0';

        bool supported = true;

        if      (strcmp(name_string, "IHDR") == 0) IHDR();
        else if (strcmp(name_string, "PLTE") == 0) PLTE();
        else if (strcmp(name_string, "IDAT") == 0) IDAT();
        else if (strcmp(name_string, "IEND") == 0) break;
        else supported = false;

        char *color   = supported ? RESET : RED;
        char *message = supported ? "" : "(Chunk not supported)\n";

        printf(
            "%s%sChunk: %s\nLength: %uB\nOffset: %zu\n\n" RESET,
            color,
            message,
            name_string,
            length,
            pointer - (uint8_t*)data
        );

        pointer += length + 12;
    }

    free(data);

    return 0;
}
