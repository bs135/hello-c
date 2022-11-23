/**
 * Copyright (c) 2022
 *
 */

#include "fs.h"
#include "utils/log/log.h"
#include "utils/split/split.h"

static pthread_mutex_t mutex_lock;

int fs_write_data(char *data, int data_size, const char *filename, fs_mode_t mode)
{
    pthread_mutex_lock(&mutex_lock);
    FILE *fp;
    fp = fopen(filename, fs_mode_code[mode]);

    if (fp == NULL)
    {
        log_error("Cannot read file: %s", filename);
        pthread_mutex_unlock(&mutex_lock);
        return -1;
    }
    fwrite(data, 1, data_size, fp);
    fclose(fp);
    pthread_mutex_unlock(&mutex_lock);
    return 0;
}

int fs_write_fmt(const char *filename, const char *fmt, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, fmt);
    int rc = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // log_debug("rc: %d", rc);
    // log_debug("buffer: %s", buffer);

    return fs_write_data(buffer, rc, filename, FS_APPEND_PLUS);
}

char *fs_read_data(const char *filename)
{
    size_t len;
    char *data;
    pthread_mutex_lock(&mutex_lock);
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        log_error("Cannot read file: %s", filename);
        pthread_mutex_unlock(&mutex_lock);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);
    pthread_mutex_unlock(&mutex_lock);
    return data;
}

char **fs_read_lines(const char *filename, int *n_lines)
{
    int num_lines;
    char *file_data = fs_read_data(filename);
    if (file_data == NULL)
    {
        log_error("Cannot read file %s", filename);
        return NULL;
    }
    char **lines = split(file_data, "\n", &num_lines);
    if (lines == NULL)
    {
        log_error("Cannot split file %s with multiple line", filename);
        return NULL;
    }
    *n_lines = num_lines;
    free(file_data);
    return lines;
}

bool fs_file_exists(const char *filename)
{
    return (access(filename, F_OK) == 0);

    struct stat sb;
    if (stat(filename, &sb) == 0)
    {
        if (sb.st_mode & F_OK)
            return true;
    }
    return false;
}

bool fs_dir_exists(const char *dir)
{
    struct stat sb;
    if (stat(dir, &sb) == 0)
    {
        if (S_ISDIR(sb.st_mode))
            return true;
    }
    return false;
}

/*
mkdir enhancements
    - recursive
    - check exists
*/
int fs_mkdir(const char *dir, const mode_t mode)
{
    char tmp[PATH_MAX];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    len = strnlen(dir, PATH_MAX);
    if (len == 0 || len >= PATH_MAX)
    {
        log_error("dir path is empty or too long (%d): %s", len, dir);
        return -1;
    }
    memcpy(tmp, dir, len);
    tmp[len] = '\0';

    /* remove trailing slash */
    if (tmp[len - 1] == '/')
    {
        tmp[len - 1] = '\0';
    }

    /* check if path exists and is a directory */
    if (stat(tmp, &sb) == 0)
    {
        if (S_ISDIR(sb.st_mode))
        {
            log_debug("Directory is existed: %s", tmp);
            return 0;
        }
    }

    /* recursive mkdir */
    for (p = tmp + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            /* test path */
            if (stat(tmp, &sb) != 0)
            {
                /* path does not exist - create directory */
                if (mkdir(tmp, mode) < 0)
                {
                    log_error("Unable to create directory: %s", tmp);
                    return -1;
                }
            }
            else if (!S_ISDIR(sb.st_mode))
            {
                /* not a directory */
                log_error("path is not directory: %s", tmp);
                return -1;
            }
            *p = '/';
        }
    }
    /* test path */
    if (stat(tmp, &sb) != 0)
    {
        /* path does not exist - create directory */
        if (mkdir(tmp, mode) < 0)
        {
            log_error("Unable to create directory: %s", tmp);
            return -1;
        }
    }
    else if (!S_ISDIR(sb.st_mode))
    {
        /* not a directory */
        log_error("path is not directory: %s", tmp);
        return -1;
    }
    log_info("directory is created successfully: %s", dir);
    return 0;
}

int fs_rm(const char *filename)
{
    int r = remove(filename);
    if (r == 0)
        log_debug("File is deleted successfully: %s", filename);
    else
        log_error("File is not deleted: %s", filename);
    return r;
}