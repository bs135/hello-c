/**
 * Copyright (c) 2022
 *
 */

#ifndef __STR_STR_H__
#define __STR_STR_H__

#include <stdbool.h>

#define str_bool(val) (val ? "true" : "false")

char *str_ltrim_dup(const char *s); // left trim
char *str_rtrim_dup(const char *s); // right trim
char *str_trim_dup(const char *s);  // both left and right trim

bool str_null_or_empty(const char *s);

bool str_start_with(const char *s, const char *prefix);
bool str_end_with(const char *s, const char *suffix);
bool str_contains(const char *s, const char *sub);

bool str_equal(const char *s1, const char *s2);

char *str_tolower(const char *s);
char *str_dup(const char *s);
char *str_ndup(const char *s, int n);

#endif
