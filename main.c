#include "colors.h"
#include "parser.h"
#include "util.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void throw_error(char *message) {
    fprintf(stderr, RED "\n%s\n" RESET, message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc == 1) throw_error("Path required");

    const size_t length = strlen(argv[1]);
    if (length > 256) throw_error("Path length exceeded 256 characters");

    char *path = malloc(length + 1);
    if (path == NULL) throw_error("Failed to allocate memory");
    strcpy(path, argv[1]);

    if (parse(path) != 0) throw_error("PNG parsing failed");

    free(path);
    return 0;
}

