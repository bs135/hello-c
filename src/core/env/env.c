/**
 * Copyright (c) 2022
 *
 */

#include "env.h"

#include "utils/utils.h"

#ifndef VERSION
#define VERSION "0.0"
#endif

#define REDIS_HOST_DEFAULT "127.0.0.1"
static char *redis_host;
char *env_redis_host()
{
    return redis_host;
}

#define REDIS_PORT_DEFAULT 6379
static int redis_port;
int env_redis_port()
{
    return redis_port;
}

#define REDIS_PASSWORD_DEFAULT ""
static char *redis_password;
char *env_redis_password()
{
    return redis_password;
}

#define REDIS_DB_DEFAULT 0
static int redis_db;
int env_redis_db()
{
    return redis_db;
}

#define REDIS_PING_INTERVAL_DEFAULT 3000
static int redis_ping_interval;
int env_redis_ping_interval()
{
    return redis_ping_interval;
}

void env_init(const char *file)
{
    if (!file)
        file = ENV_FILE_DEFAULT;
    int err = env_load(file, false);
    log_error_if(err != 0, "Unable to load env file");

    redis_host = env_get("REDIS_HOST", REDIS_HOST_DEFAULT);
    redis_port = env_get_int("REDIS_PORT", REDIS_PORT_DEFAULT);
    redis_password = env_get("REDIS_PASSWORD", REDIS_PASSWORD_DEFAULT);
    redis_db = env_get_int("REDIS_DB", REDIS_DB_DEFAULT);

    redis_ping_interval = env_get_int("REDIS_PING_INTERVAL", REDIS_PING_INTERVAL_DEFAULT);
}
