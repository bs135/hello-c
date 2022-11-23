/**
 * Copyright (c) 2022
 *
 */

#ifndef __JSONX_JSONX_H__
#define __JSONX_JSONX_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <json-c/json.h>

#include "utils/log/log.h"
#include "utils/str/str.h"

#define JSONX_PRETTY_PRINT

struct json_object *jsonx_from_file(char *filename);
int jsonx_to_file(char *filename, json_object *obj);

struct json_object *jsonx_from_string(const char *str);
char *jsonx_to_string(json_object *obj);

#define jsonx_free(obj) \
    json_object_put(obj)

#define jsonx_parse(str, ret_obj) \
    json_object *ret_obj = json_tokener_parse((const char *)str);

#define jsonx_get_object(obj, key, ret_obj)                                                                \
    json_object *ret_obj;                                                                                  \
    if (!json_object_object_get_ex(obj, key, &ret_obj) || !json_object_is_type(ret_obj, json_type_object)) \
        ret_obj = NULL;

#define jsonx_get_str(obj, key, ret_val)                                                                               \
    json_object *_obj_##ret_val;                                                                                       \
    char *ret_val = NULL;                                                                                              \
    if (json_object_object_get_ex(obj, key, &_obj_##ret_val) && json_object_is_type(_obj_##ret_val, json_type_string)) \
        ret_val = (char *)json_object_get_string(_obj_##ret_val);

#define jsonx_get_int(obj, key, ret_val)                                                                            \
    json_object *_obj_##ret_val;                                                                                    \
    int ret_val = -1;                                                                                               \
    if (json_object_object_get_ex(obj, key, &_obj_##ret_val) && json_object_is_type(_obj_##ret_val, json_type_int)) \
        ret_val = json_object_get_int(_obj_##ret_val);

#define jsonx_get_int64(obj, key, ret_val)                                                                          \
    json_object *_obj_##ret_val;                                                                                    \
    int64_t ret_val = -1;                                                                                           \
    if (json_object_object_get_ex(obj, key, &_obj_##ret_val) && json_object_is_type(_obj_##ret_val, json_type_int)) \
        ret_val = json_object_get_int64(_obj_##ret_val);

#define jsonx_get_bool(obj, key, ret_val)                                                                               \
    json_object *_obj_##ret_val;                                                                                        \
    bool ret_val = false;                                                                                               \
    if (json_object_object_get_ex(obj, key, &_obj_##ret_val) && json_object_is_type(_obj_##ret_val, json_type_boolean)) \
        ret_val = json_object_get_boolean(_obj_##ret_val);

#endif
