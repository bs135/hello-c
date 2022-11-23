/**
 * Copyright (c) 2022
 *
 */

#include "dev_info.h"

#include "core/core.h"
#include "utils/utils.h"

/**
 * @brief Get WAN IPv4
 * - https://openwrt.org/docs/guide-developer/network-scripting
 *
 * @return char*
 */
char *dev_wan_gw()
{
    if (fs_file_exists(DEV_GET_WAN_GW_SCRIPT))
    {
        cmd_ret_t ret = cmd_execl_timeout(200, DEV_GET_WAN_GW_SCRIPT);
        if (ret.err == CMD_OK)
        {
            char *s = str_trim_dup(ret.str);
            free(ret.str);
            return s;
        }
    }
    else
    {
        log_warn("File not found: %s", DEV_GET_WAN_GW_SCRIPT);
        return "";
    }
    return NULL;
}

/**
 * @brief Get WAN IPv6
 * - https://openwrt.org/docs/guide-developer/network-scripting
 *
 * @return char*
 */
char *dev_wan_gw6()
{
    if (fs_file_exists(DEV_GET_WAN_GW6_SCRIPT))
    {
        cmd_ret_t ret = cmd_execl_timeout(200, DEV_GET_WAN_GW6_SCRIPT);
        if (ret.err == CMD_OK)
        {
            char *s = str_trim_dup(ret.str);
            free(ret.str);
            return s;
        }
    }
    else
    {
        log_warn("File not found: %s", DEV_GET_WAN_GW6_SCRIPT);
    }
    return NULL;
}
