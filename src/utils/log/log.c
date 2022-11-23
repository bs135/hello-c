/*
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "log.h"

#define LOG_MAX_CALLBACKS 32
#define LOG_MAX_LENGTH 2048

typedef struct
{
    log_LogFn fn;
    void *udata;
    int level;
} log_Callback;

static struct
{
    void *udata;
    log_LockFn lock;
    int level;
    bool quiet;
    const char *tag;
    log_Callback callbacks[LOG_MAX_CALLBACKS];
} L;

static const char *level_strings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

static int levels_len = (sizeof(level_strings) / sizeof(level_strings[0]));

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"};
#endif

static void stdout_callback(log_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(
        ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d,%s():\x1b[0m ",
        buf, level_colors[ev->level], level_strings[ev->level],
        ev->file, ev->line, ev->func);
#else
    fprintf(
        ev->udata, "%s %-5s %s:%d,%s(): ",
        buf, level_strings[ev->level], ev->file, ev->line, ev->func);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void file_callback(log_Event *ev)
{
    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    fprintf(
        ev->udata, "%s %-5s %s:%d: ",
        buf, level_strings[ev->level], ev->file, ev->line);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static char syslog_msg[LOG_MAX_LENGTH];
static void syslog_callback(log_Event *ev)
{
    snprintf(syslog_msg, sizeof(syslog_msg),
             "[%s] %-5s %s:%d: ",
             L.tag ? L.tag : "log", level_strings[ev->level], ev->file, ev->line);

    vsnprintf(&syslog_msg[strlen(syslog_msg)], sizeof(syslog_msg), ev->fmt, ev->ap);

    syslog(syslog_lv_map[ev->level], "%s", syslog_msg);
}

static void lock(void)
{
    if (L.lock)
    {
        L.lock(true, L.udata);
    }
}

static void unlock(void)
{
    if (L.lock)
    {
        L.lock(false, L.udata);
    }
}

const char *log_level_string(int level)
{
    if (level >= 0 && level < levels_len)
        return level_strings[level];
    return "";
}

void log_set_lock(log_LockFn fn, void *udata)
{
    L.lock = fn;
    L.udata = udata;
}

void log_set_level(int level)
{
    if (level >= 0 && level < levels_len)
        L.level = level;
}

void log_set_tag(const char *tag)
{
    if (tag)
        L.tag = __name(tag);
}

void log_set_quiet(bool enable)
{
    L.quiet = enable;
}

int log_add_callback(log_LogFn fn, void *udata, int level)
{
    for (int i = 0; i < LOG_MAX_CALLBACKS; i++)
    {
        if (!L.callbacks[i].fn)
        {
            L.callbacks[i] = (log_Callback){fn, udata, level};
            return 0;
        }
    }
    return -1;
}

int log_add_fp(FILE *fp, int level)
{
    return log_add_callback(file_callback, fp, level);
}

static void init_event(log_Event *ev, void *udata)
{
    if (!ev->time)
    {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

void log_init(char *tag, int log_level)
{
    log_set_tag(__name(tag));
    log_set_level(log_level);

#if LOG_USE_SYSLOG
    openlog(__name(tag), LOG_CONS | LOG_PID, LOG_LOCAL1);
    setlogmask(LOG_UPTO(syslog_lv_map[log_level]));
#endif
}

void log_log(int level, const char *file, int line, const char *func, const char *fmt, ...)
{
    log_Event ev = {
        .fmt = fmt,
        .file = file,
        .line = line,
        .func = func,
        .level = level,
    };

    lock();

    if (!L.quiet && level >= L.level)
    {
        init_event(&ev, stderr);
        va_start(ev.ap, fmt);
        stdout_callback(&ev);
        va_end(ev.ap);
    }

#if LOG_USE_SYSLOG
    if (!L.quiet && level >= L.level)
    {
        init_event(&ev, NULL);
        va_start(ev.ap, fmt);
        syslog_callback(&ev);
        va_end(ev.ap);
    }
#endif

    for (int i = 0; i < LOG_MAX_CALLBACKS && L.callbacks[i].fn; i++)
    {
        log_Callback *cb = &L.callbacks[i];
        if (level >= cb->level)
        {
            init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

int log_level_number(const char *level)
{
    int i;
    for (i = 0; i < levels_len; i++)
    {
        if (strcasecmp(level_strings[i], level) == 0)
        {
            return i;
        }
    }
    return -1;
}

void log_final()
{
#if LOG_USE_SYSLOG
    closelog();
#endif
}
