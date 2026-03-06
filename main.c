#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void throw_error(char *message) {
    fprintf(stderr, "\033[38;5;196m%s\033[0m\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc == 1) throw_error("Path required");

    const size_t length = strlen(argv[1]);
    if (length > 256) throw_error("Path length exceeded 256 characters");

    char *path = malloc(length + 1);
    strcpy(path, argv[1]);

    render(path);

    free(path);

    return 0;
}

