/**
 * Copyright (c) 2022
 *
 */

#include <sys/time.h>
#include <stdlib.h>
#include "utils/log/log.h"

long long now_ms()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long ms = te.tv_sec * 1000LL + te.tv_usec / 1000;
    // log_debug("time: %lld", ms);
    return ms;
}
