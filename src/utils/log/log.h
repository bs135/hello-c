/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef __LOG_LOG_H__
#define __LOG_LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <syslog.h>
#include <string.h>

typedef struct
{
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    void *udata;
    int line;
    const char *func;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum
{
    LOGGER_TRACE,
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_FATAL
};

static const int syslog_lv_map[] = {
    [LOGGER_DEBUG] = LOG_DEBUG,
    [LOGGER_INFO] = LOG_INFO,
    // [LOGGER_INFO] = LOG_NOTICE,
    [LOGGER_WARN] = LOG_WARNING,
    [LOGGER_ERROR] = LOG_ERR,
    [LOGGER_FATAL] = LOG_CRIT,
    // [LOGGER_FATAL] = LOG_ALERT,
    // [LOGGER_FATAL] = LOG_EMERG,
};

#define __name(path) (strrchr(path, '/') ? strrchr(path, '/') + 1 : path)

#define log_trace(...) log_log(LOGGER_TRACE, __name(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_debug(...) log_log(LOGGER_DEBUG, __name(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_info(...) log_log(LOGGER_INFO, __name(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_warn(...) log_log(LOGGER_WARN, __name(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_log(LOGGER_ERROR, __name(__FILE__), __LINE__, __func__, __VA_ARGS__)

#define log_fatal(...)                                                            \
    do                                                                            \
    {                                                                             \
        log_log(LOGGER_FATAL, __name(__FILE__), __LINE__, __func__, __VA_ARGS__); \
        exit(EXIT_FAILURE);                                                       \
    } while (0)

#define log_warn_if(expr, ...)     \
    do                             \
    {                              \
        if (expr)                  \
        {                          \
            log_warn(__VA_ARGS__); \
        }                          \
    } while (0)

#define log_error_if(expr, ...)     \
    do                              \
    {                               \
        if (expr)                   \
        {                           \
            log_error(__VA_ARGS__); \
        }                           \
    } while (0)

#define log_fatal_if(expr, ...)     \
    do                              \
    {                               \
        if (expr)                   \
        {                           \
            log_fatal(__VA_ARGS__); \
        }                           \
    } while (0)

const char *log_level_string(int level);
int log_level_number(const char *level);

void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_tag(const char *tag);
void log_set_quiet(bool enable);

int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *func, const char *fmt, ...);

void log_init(char *tag, int log_level);
void log_final();

#endif
