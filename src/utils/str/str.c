/**
 * Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "str.h"

char *str_ltrim_dup(const char *s)
{
    if (s == NULL)
        return NULL;
    int l = strlen(s);
    while (*s && isspace(*s))
        ++s, --l;
    return strndup(s, l);
}

char *str_rtrim_dup(const char *s)
{
    if (s == NULL)
        return NULL;
    int l = strlen(s);
    while (isspace(s[l - 1]))
        --l;
    return strndup(s, l);
}

char *str_trim_dup(const char *s)
{
    // return str_ltrim_dup(str_rtrim_dup(s));
    if (s == NULL)
        return NULL;
    int l = strlen(s);
    while (*s && isspace(*s))
        ++s, --l;
    while (isspace(s[l - 1]))
        --l;
    return strndup(s, l);
}

bool str_null_or_empty(const char *s)
{
    return (s == NULL || s[0] == '\0');
}

bool str_start_with(const char *s, const char *prefix)
{
    if (!s || !prefix)
        return false;
    size_t len_prefix = strlen(prefix);
    size_t len_str = strlen(s);
    if (len_str < len_prefix)
        return false;
    return strncmp(prefix, s, len_prefix) == 0;
}

bool str_end_with(const char *s, const char *suffix)
{
    if (!s || !suffix)
        return false;
    size_t len_suffix = strlen(suffix);
    size_t len_str = strlen(s);
    if (len_suffix > len_str)
        return false;
    return strncmp(s + len_str - len_suffix, suffix, len_suffix) == 0;
}

bool str_contains(const char *s, const char *sub)
{
    if (!s || !sub)
        return false;
    return strstr(s, sub) != NULL;
}

bool str_equal(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return false;
    return strcmp(s1, s2) == 0;
}

char *str_tolower(const char *s)
{
    if (!s)
        return NULL;
    int len = strlen(s);
    char *ret = (char *)malloc(len);
    strcpy(ret, s);
    for (size_t i = 0; i < len; i++)
    {
        ret[i] = tolower(ret[i]);
    }
    return ret;
}

char *str_dup(const char *s)
{
    if (!s)
        return NULL;
    return strdup(s);
}

char *str_ndup(const char *s, int n)
{
    if (!s)
        return NULL;
    return strndup(s, n);
}
