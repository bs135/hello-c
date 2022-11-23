/**
 * Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "utils/timer/timer.h"
#include "utils/queue/queue.h"
#include "utils/log/log.h"
#include "go.h"

#define GO_THREAD_GARBAGE_INTERVAL 50
#define GO_THREAD_QUEUE_MAX_SIZE 1000

static pthread_mutex_t _pthread_mux;
Queue_t _thread_queue;

/**
 * @brief check and release garbage thread periodically
 *
 * @param user_data
 */
void _go_thread_garbage(void *user_data)
{
    if (!q_isEmpty(&_thread_queue))
    {
        pthread_t new_thread;
        pthread_mutex_lock(&_pthread_mux);
        q_pop(&_thread_queue, &new_thread);
        if (pthread_tryjoin_np(new_thread, NULL) != 0)
        {
            q_push(&_thread_queue, &new_thread);
        }
        else
        {
            // log_debug("pop thread");
        }
        pthread_mutex_unlock(&_pthread_mux);
    }
}

/**
 * @brief init thread queue ans start thread_garbage task
 *
 */
void go_init()
{
    q_init(&_thread_queue, sizeof(pthread_t), GO_THREAD_QUEUE_MAX_SIZE, FIFO, true);
    timer_start(GO_THREAD_GARBAGE_INTERVAL, _go_thread_garbage, TIMER_PERIODIC, NULL);
}

/**
 * @brief run the routine in background
 *
 * @param routine
 * @param arg
 */
void go(go_routine_t routine, void *arg)
{
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, routine, arg);
    pthread_mutex_lock(&_pthread_mux);
    q_push(&_thread_queue, &new_thread);
    pthread_mutex_unlock(&_pthread_mux);
}
