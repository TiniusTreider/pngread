#include "chunks.h"
#include "colors.h"
#include "error.h"
#include "io.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



static inline char *bool_to_string(bool value)
{
    return value ? "true" : "false";
}

void print_chunk_header(struct chunk *chunk)
{
    printf(
        "\n"
        "Name: ........ %s\n"
        "Length: ...... %uB\n"
        "Critical: .... %s\n"
        "Supported: ... %s\n",
        chunk->name,
        chunk->length,
        bool_to_string(chunk->critical),
        bool_to_string(chunk->supported)
    );
}



#define IHDR_WIDTH_OFFSET            0
#define IHDR_HEIGHT_OFFSET           4
#define IHDR_BIT_DEPTH_OFFSET        8
#define IHDR_COLOR_TYPE_OFFSET       9
#define IHDR_INTERLACE_METHOD_OFFSET 12

struct image_header
{
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t interlace_method;
};

static inline void print_image_header(struct image_header header)
{
    const char *color_type;
    switch (header.color_type)
    {
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
            throw_error("Invalid color type (must be 0, 2, 3, 4, 6)");
    }

    const char *interlace_method = header.interlace_method == 1 ? "Adam7" : "None";

    printf(
        " - width: ......... %u\n"
        " - height: ........ %u\n"
        " - bit depth: ..... %u\n"
        " - color: ......... %s\n"
        " - Interlace: ..... %s\n",
        header.width,
        header.height,
        header.bit_depth,
        color_type,
        interlace_method
    );
}

void IHDR(uint8_t *data, uint32_t length)
{
    throw_error_if(length != 13, "Invalid IHDR chunk data length (must be 13B)");

    struct image_header header;
    header.width  = read_big_endian_uint32(data, IHDR_WIDTH_OFFSET);
    header.height = read_big_endian_uint32(data, IHDR_HEIGHT_OFFSET);
    header.bit_depth        = data[IHDR_BIT_DEPTH_OFFSET];
    header.color_type       = data[IHDR_COLOR_TYPE_OFFSET];
    header.interlace_method = data[IHDR_INTERLACE_METHOD_OFFSET];

    print_image_header(header);
}



void PLTE(uint8_t *data, uint32_t length)
{
    throw_error_if(length < 3 || length > 768, "Invalid PLTE chunk data length (must be 3 - 768)");
    throw_error_if(length % 3 != 0, "Invalid PLTE chunk data length (must be a multiple of three");
}



void IDAT(uint8_t *data, uint32_t length)
{
    //WIP
}



void IEND(uint8_t *data, uint32_t length)
{
    //WIP
}



const struct chunk_function chunk_functions[] = {
    (struct chunk_function){ "IHDR", IHDR },
    (struct chunk_function){ "PLTE", PLTE },
    (struct chunk_function){ "IDAT", IDAT },
    (struct chunk_function){ "IEND", IEND }
};

