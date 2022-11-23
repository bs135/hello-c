#include <uuid/uuid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uuid.h"

char *uuid_gen(const char *prefix)
{
    uuid_t binuuid;

    uuid_generate_random(binuuid);

    char *uuid = malloc(UUID_STR_LEN);
    uuid_unparse_lower(binuuid, uuid);

    /* if prefix is NULL or empty */
    if (prefix == NULL || prefix[0] == '\0')
    {
        // printf("id: %s\n", uuid);
        return uuid;
    }

    /* else */
    char *ret_uuid = (char *)malloc(UUID_STR_LEN + strlen(prefix) + 1);
    sprintf(ret_uuid, "%s_%s", prefix, uuid);
    free(uuid);
    // log_info("id: %s\n", ret_uuid);
    return ret_uuid;
}
