/**
 * Copyright (c) 2022
 *
 */

#ifndef __GO_GO_H__
#define __GO_GO_H__

typedef void *(*go_routine_t)(void *arg);

void go_init();
void go(go_routine_t routine, void *arg);

#endif