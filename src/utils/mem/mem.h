#ifndef __MEM_MEM_H__
#define __MEM_MEM_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void mem_free(void *ptr);
bool mem_is_empty(void *ptr, size_t size);

#endif