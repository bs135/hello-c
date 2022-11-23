/**
 * Copyright (c) 2022
 *
 * https://github.com/soywod/c-map/
 */

#include <stdlib.h>
#include <string.h>

#include "map.h"

#define MAP_BY_VAL 0
#define MAP_BY_REF 1

map_t *map_new(size_t capacity)
{
    map_t *map;

    map = malloc(sizeof(map_t));
    map->capacity = capacity;
    map->size = 0;
    map->items = NULL;

    return map;
}

map_err_t map_add(char *key, void *val, map_t *map)
{
    if (!map || !key)
    {
        return MAP_ERR_NULL;
    }

    if (map->size >= map->capacity)
    {
        return MAP_ERR_EXCEEDED_LIMIT;
    }

    if (map_get(key, map))
    {
        return MAP_ERR_KEY_EXISTED;
    }

    char *newkey;

    newkey = malloc(strlen(key) + 1);
    strcpy(newkey, key);

    if (map->size == 0)
    {
        map->items = malloc(sizeof(map_item_t));
    }
    else
    {
        map->items = realloc(map->items, sizeof(map_item_t) * (map->size + 1));
    }

    (map->items + map->size)->key = newkey;
    (map->items + map->size)->val = val;
    (map->items + map->size++)->type = MAP_BY_VAL;

    return MAP_OK;
}

void map_dyn_add(char *key, void *val, map_t *map)
{
    map_add(key, val, map);
    (map->items + map->size - 1)->type = MAP_BY_REF;
}

void *map_get(char *key, map_t *map)
{
    int i;
    for (i = 0; i < map->size; i++)
    {
        if (strcmp((map->items + i)->key, key) == 0)
        {
            return (map->items + i)->val;
        }
    }
    return NULL;
}

int strcmp_hashtag(const char *pattern, const char *key)
{
    char *hashtag;
    size_t index;
    hashtag = strchr(pattern, '#');
    index = (size_t)(hashtag - pattern);
    if (index > 0 && index < strlen(pattern))
    {
        return strncmp(pattern, key, index);
    }
    return strcmp(pattern, key);
}

void *map_get_hashtag(char *key, map_t *map)
{
    int i;
    for (i = 0; i < map->size; i++)
    {
        if (strcmp_hashtag((map->items + i)->key, key) == 0)
        {
            return (map->items + i)->val;
        }
    }
    return NULL;
}

void map_free(map_t *map)
{
    int i = 0;

    for (; i < map->size; i++)
    {
        free((map->items + i)->key);

        if ((map->items + i)->type == MAP_BY_REF)
        {
            free((map->items + i)->val);
        }
    }

    free(map->items);
    free(map);
}
