#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * data;


const char NORMAL_SIGNATURE[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
static inline void signature(void) {
    if (memcmp(NORMAL_SIGNATURE, data, 8) != 0) {
        throw_error("Invalid PNG");
    } else {
        printf("Valid PNG\n");
    }
}

static inline void IHDR(void) {
    //WIP
}

static inline void PLTE(void) {
    //WIP
}

static inline void IDAT(void) {
    //WIP
}

static inline void IEND(void) {
    //WIP
}

struct chunk_name {
    char *name;
    void (*function)(void);
};
int render(char *path) {
    const size_t data_length = (size_t)file_length(path);
    data = malloc(data_length);
    if (data == NULL) throw_error("Failed to allocate memory");
    read_file(path, data_length, data);

    signature();
    
    void *pointer = data + 8;
    void *end_pointer = data + data_length;
    while (pointer <= end_pointer) {
        
    }

    free(data);

    return 0;
}
