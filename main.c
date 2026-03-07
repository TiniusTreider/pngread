#include "colors.h"
#include "parser.h"
#include "util.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_PATH_LENGTH 256

int main(int argc, char *argv[]) {
    if (argc == 1) throw_error("Path required");

    const size_t length = strlen(argv[1]);
    if (length > MAX_PATH_LENGTH) throw_error("Path length exceeded 256 characters");

    if (parse(argv[1]) != 0) throw_error("PNG parsing failed");

    return 0;
}

