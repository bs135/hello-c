#ifndef __SPLIT_SPLIT_H__
#define __SPLIT_SPLIT_H__

#include <string.h>

char **split(char *str, char *pattern, int *size);
void split_free(char **split_ret, size_t size);

#endif