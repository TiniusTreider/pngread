#include "colors.h"
#include "parser.h"
#include "error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_PATH_LENGTH 256

int main(int argc, char *argv[])
{
    throw_error_if(argc == 1, "Path required");

    const size_t length = strlen(argv[1]);
    throw_error_if(length > MAX_PATH_LENGTH, "Path length exceeded 256 characters");

    parse(argv[1]);

    return 0;
}

