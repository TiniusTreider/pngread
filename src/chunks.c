#include "chunks.h"
#include "error.h"
#include "io.h"

#include <stdint.h>
#include <stdio.h>



#define IHDR_WIDTH_OFFSET            0
#define IHDR_HEIGHT_OFFSET           4
#define IHDR_BIT_DEPTH_OFFSET        8
#define IHDR_COLOR_TYPE_OFFSET       9
#define IHDR_INTERLACE_METHOD_OFFSET 12

struct image_header {
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t interlace_method;
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

    const char *Adam7;
    switch (header.interlace_method) {
        case 0:
            Adam7 = "false"; break;
        case 1:
            Adam7 = "true"; break;
        default:
            throw_error("Invalid interlace method");
    }

    printf(
        " - width: %u\n - height: %u\n - bit depth: %u\n - color: %s\n - Adam7: %s\n",
        header.width,
        header.height,
        header.bit_depth,
        color_type,
        Adam7
    );
}

void IHDR(uint8_t *data, uint32_t length) {
    struct image_header header;
    header.width  = read_big_endian_uint32(data, IHDR_WIDTH_OFFSET);
    header.height = read_big_endian_uint32(data, IHDR_HEIGHT_OFFSET);
    header.bit_depth        = data[IHDR_BIT_DEPTH_OFFSET];
    header.color_type       = data[IHDR_COLOR_TYPE_OFFSET];
    header.interlace_method = data[IHDR_INTERLACE_METHOD_OFFSET];

    print_image_header(header);
}



void PLTE(uint8_t *data, uint32_t length) {
    //WIP
}



void IDAT(uint8_t *data, uint32_t length) {
    //WIP
}



void IEND(uint8_t *data, uint32_t length) {
    //WIP
}



const chunk_function chunk_functions[] = {
    (chunk_function){ "IHDR", IHDR },
    (chunk_function){ "PLTE", PLTE },
    (chunk_function){ "IDAT", IDAT },
    (chunk_function){ "IEND", IEND }
};

