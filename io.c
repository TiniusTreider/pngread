#include "header.h"

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

