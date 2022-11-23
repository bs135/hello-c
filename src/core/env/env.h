/**
 * Copyright (c) 2022
 *
 */

#ifndef __ENV_ENV_H__
#define __ENV_ENV_H__

#include <stdio.h>

#define ENV_FILE_DEFAULT "/data/hello-world/.env"

char *env_redis_host();
int env_redis_port();
char *env_redis_password();
int env_redis_db();

int env_redis_ping_interval();

void env_init(const char *file);

#endif