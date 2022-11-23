/**
 * Copyright (c) 2022
 *
 */

#ifndef __DEV_INFO_H__
#define __DEV_INFO_H__

#include <stdio.h>

#define DEV_GET_WAN_GW_SCRIPT "/etc/hello-world/script/get_wan_gw.sh"
#define DEV_GET_WAN_GW6_SCRIPT "/etc/hello-world/script/get_wan_gw6.sh"

char *dev_wan_gw();
char *dev_wan_gw6();

#endif
