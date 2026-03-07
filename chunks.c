#include "chunks.h"
#include "util.h"

#include <stdint.h>
#include <stdio.h>



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
    header.width  = read_big_endian_uint32(data, 0);
    header.height = read_big_endian_uint32(data, 1);
    header.bit_depth        = data[8];
    header.color_type       = data[9];
    header.interlace_method = data[12];

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

