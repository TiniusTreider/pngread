#include "colors.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>


void throw_error(char *message) {
    fprintf(stderr, RED "\n%s\n" RESET, message);
    exit(EXIT_FAILURE);
}

void *safe_malloc(size_t size) {
    void *pointer = malloc(size);
    if (pointer == NULL) throw_error("Failed to allocate memory");
    return pointer;
}

