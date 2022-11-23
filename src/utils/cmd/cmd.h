#ifndef __CMD_CMD_H__
#define __CMD_CMD_H__

typedef enum
{
    CMD_OK,
    CMD_TIMEOUT,
    CMD_FAIL
} cmd_err_t;

typedef struct
{
    cmd_err_t err;
    char *str;
} cmd_ret_t;

cmd_ret_t cmd_execl_timeout(int timeout_ms, const char *cmd);
void cmd_execl(const char *cmd);
void cmd_execl_file(const char *path_to_file);

#endif