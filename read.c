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

struct image_header {
    int32_t width;
    int32_t height;
    int8_t bit_depth;
    int8_t color_type;
    int8_t interlace_method;
};
static inline void print_image_header(struct image_header header) {
    const char *color_type;
    switch (header.color_type) {
        case 0:
            color_type = "Grayscale"; break;
        case 2:
            color_type = "RGB"; break;
        case 3:
            color_type = "Palette"; break;
        case 4:
            color_type = "Grayscale + Alpha"; break;
        case 6:
            color_type = "RGB + Alpha"; break;
        default:
            throw_error("Invalid color type");
    }
    printf(
        " - width: %u\n - height: %u\n - bit depth: %u\n - color: %s\n - Adam7: %d\n",
        header.width,
        header.height,
        header.bit_depth,
        color_type,
        header.interlace_method
    );
}
static inline void IHDR(uint8_t *data, uint32_t length) {
    struct image_header header;
    header.width  = read_big_endian_uint32(data, 0);
    header.height = read_big_endian_uint32(data, 1);
    header.bit_depth        = data[8];
    header.color_type       = data[9];
    header.interlace_method = data[12];

    print_image_header(header);
}

static inline void PLTE(uint8_t *data, uint32_t length) {
    //WIP
}

static inline void IDAT(uint8_t *data, uint32_t length) {
    //WIP
}

static inline void IEND(uint8_t *data, uint32_t length) {
    //WIP
}

int render(char *path) {
    const size_t data_length = (size_t)file_length(path);
    data = malloc(data_length);
    if (data == NULL) throw_error("Failed to allocate memory");
    read_file(path, data_length, data);

    signature();
    printf("Size: %.2fkB\n", (float)data_length / 1000);

    uint8_t *pointer     = (uint8_t*)data + 8;
    uint8_t *end_pointer = (uint8_t*)data + data_length;
    while (pointer < end_pointer) {
        const uint32_t length = read_big_endian_uint32(pointer, 0);

        char name_string[5];
        memcpy(name_string, pointer + 4, 4);
        if (name_string[2] & 0b00100000) throw_error("Malformed chunk");
        name_string[4] = '\0';

        bool supported = true;
        void (*chunk_function)(uint8_t*, uint32_t);
        if      (strcmp(name_string, "IHDR") == 0) chunk_function = IHDR;
        else if (strcmp(name_string, "PLTE") == 0) chunk_function = PLTE;
        else if (strcmp(name_string, "IDAT") == 0) chunk_function = IDAT;
        else if (strcmp(name_string, "IEND") == 0) chunk_function = IEND;
        else supported = false;

        char *color   = supported ? RESET : RED;
        char *message = supported ? "" : "(Chunk not supported)\n";
        printf(
            "\n%s%sChunk: %s\nLength: %uB\nOffset: %zu\n" RESET,
            color,
            message,
            name_string,
            length,
            pointer - (uint8_t*)data
        );

        if (supported) chunk_function(pointer + 8, length);

        pointer += length + 12;
    }

    free(data);

    printf(GREEN "\nSuccess!\n" RESET);

    return 0;
}
