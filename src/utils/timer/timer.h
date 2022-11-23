#ifndef __TIMER_TIMER_H__
#define __TIMER_TIMER_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/timerfd.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef enum
{
    TIMER_SINGLE_SHOT = 0,
    TIMER_PERIODIC
} t_timer;

typedef void (*time_handler)(void *user_data);

int timer_init();
size_t timer_start(unsigned int interval, time_handler handler, t_timer type, void *user_data);
void timer_stop(size_t timer_id);
void timer_final();

#endif
