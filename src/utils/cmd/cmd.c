#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include "utils/log/log.h"
#include "utils/timer/timer.h"
#include "utils/cmd/cmd.h"
#include "utils/fs/fs.h"

void _cmd_timeout_cb(void *user_data)
{
    bool *flag = (bool *)(user_data);
    // log_info("timer cb");
    (*flag) = true;
}

cmd_ret_t cmd_execl_timeout(int timeout_ms, const char *cmd)
{
    cmd_ret_t ret;
    FILE *fp;
    /* Open the command for reading. */
    char *new_cmd = (char *)malloc(strlen(cmd) + 10);
    // Append stderr to stdout with 2>&1
    sprintf(new_cmd, "%s 2>&1", cmd);
    fp = popen(new_cmd, "r");
    if (fp == NULL)
    {
        log_error("Failed to run command: %s", new_cmd);
        ret.err = CMD_FAIL;
        return ret;
    }
    int fd = fileno(fp);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    /* Read the output a line at a time - output it. */
    bool *timeout_flag = (bool *)malloc(sizeof(bool));
    *timeout_flag = false;
    size_t time_id = timer_start(timeout_ms, _cmd_timeout_cb, TIMER_SINGLE_SHOT, timeout_flag);
    const int buf_size = 4096;
    int curr_size = buf_size;
    char *out = malloc(curr_size);
    char c;
    int i = 0;
    do
    {
        c = fgetc(fp);
        if (feof(fp))
        {
            break;
        }
        if (curr_size - i < 5)
        {
            curr_size += buf_size;
            out = (char *)realloc(out, curr_size);
        }
        out[i++] = c;
    } while (!(*timeout_flag));
    timer_stop(time_id);
    pclose(fp);
    out[i] = 0;
    if (*timeout_flag)
    {
        log_info("timeout");
    }
    ret.err = (*timeout_flag) ? CMD_TIMEOUT : CMD_OK;
    ret.str = out;
    free(new_cmd);
    free(timeout_flag);
    return ret;
}

void cmd_execl(const char *cmd)
{
    FILE *fp;
    char *new_cmd = (char *)malloc(strlen(cmd) + 10);
    // Append stderr to stdout with 2>&1
    sprintf(new_cmd, "%s 2>&1", cmd);
    /* Open the command for reading. */
    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        log_error("Failed to run command: %s", cmd);
    }
    else
    {
        pclose(fp);
    }
}

void cmd_execl_file(const char *filepath)
{
    int nb_lines;
    char **lines = fs_read_lines(filepath, &nb_lines);
    cmd_ret_t ret;
    if (nb_lines == 0 || lines == NULL)
    {
        log_error("file %s is empty or isn't exist", filepath);
    }
    for (size_t i = 0; i < nb_lines; i++)
    {
        /* code */
        cmd_execl(lines[i]);
    }

    free(lines);
    return;
}