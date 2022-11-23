
/**
 * Copyright (c) 2022
 *
 */

#ifndef __FS_FS_H__
#define __FS_FS_H__

#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <fcntl.h>

// #include <linux/limits.h>
#define PATH_MAX 4096

typedef enum
{
    /* r - Opens a file for reading. The file must exist. */
    FS_READ = 0,

    /* w - Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file. */
    FS_WRITE,

    /* a - Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist. */
    FS_APPEND,

    /* r+ - Opens a file to update both reading and writing. The file must exist. */
    FS_READ_PLUS,

    /* w+ - Creates an empty file for both reading and writing. */
    FS_WRITE_PLUS,

    /* a+ - Opens a file for reading and appending. */
    FS_APPEND_PLUS,

} fs_mode_t;

static const char *const fs_mode_code[] = {
    [FS_READ] = "r",
    [FS_WRITE] = "w",
    [FS_APPEND] = "a",
    [FS_READ_PLUS] = "r+",
    [FS_WRITE_PLUS] = "w+",
    [FS_APPEND_PLUS] = "a+",
};

bool fs_file_exists(const char *filename);
bool fs_dir_exists(const char *dirname);

int fs_write_data(char *data, int data_size, const char *filename, fs_mode_t mode);
char *fs_read_data(const char *filename);

char **fs_read_lines(const char *filename, int *n_lines);
int fs_write_fmt(const char *filename, const char *fmt, ...);

int fs_mkdir(const char *dir, const mode_t mode);
int fs_rm(const char *filename);

#endif
