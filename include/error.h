#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stddef.h>



void throw_error(char*);
void throw_error_if(bool, char*);

void *safe_malloc(size_t);

#endif

