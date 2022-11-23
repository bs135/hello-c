/**
 * Copyright (c) 2022
 *
 * https://github.com/soywod/c-map/
 */

#ifndef __MAP_MAP_H__
#define __MAP_MAP_H__

#include <stdlib.h>

typedef enum
{
    MAP_OK = 0,
    MAP_ERR_NULL,
    MAP_ERR_EXCEEDED_LIMIT,
    MAP_ERR_KEY_EXISTED,
} map_err_t;

typedef struct
{
    char *key;
    void *val;
    int type;
} map_item_t;

typedef struct
{
    size_t capacity;
    size_t size;
    map_item_t *items;
} map_t;

map_t *map_new(size_t capacity);
map_err_t map_add(char *key, void *val, map_t *map);
void map_dyn_add(char *key, void *val, map_t *map);
void *map_get(char *key, map_t *map);
void *map_get_hashtag(char *key, map_t *map);
void map_free(map_t *map);

// TODO: remove an item of map
// void map_rm(char *key, map_t *map);

#endif
