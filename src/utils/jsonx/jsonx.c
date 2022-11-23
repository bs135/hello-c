/**
 * Copyright (c) 2022
 *
 */

#include "jsonx.h"

#ifdef JSONX_PRETTY_PRINT
#define JSONX_FORMAT JSON_C_TO_STRING_PRETTY
#else
#define JSONX_FORMAT JSON_C_TO_STRING_SPACED
#endif

/*****************************************************/
/* Function for convert between object and file      */
/*****************************************************/
json_object *jsonx_from_file(char *filename)
{
    if (!filename)
        return NULL;
    json_object *jobj = json_object_from_file(filename);
    return (json_object_get_type(jobj) == json_type_object) ? jobj : NULL;
}

int jsonx_to_file(char *filename, json_object *obj)
{
    if (!filename)
        return -1;
    // 0: pass , -1: fail
    return json_object_to_file_ext(filename, obj, JSONX_FORMAT);
}

/*****************************************************/
/* 	Function for convert between object and string 	 */
/*****************************************************/
json_object *jsonx_from_string(const char *str)
{
    return json_tokener_parse(str);
}

char *jsonx_to_string(json_object *obj)
{
    return (char *)json_object_to_json_string_ext(obj, JSONX_FORMAT);
}
