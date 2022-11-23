/**
 * Copyright (c) 2022
 *
 */

#include <syslog.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

#include "core/core.h"
#include "utils/utils.h"

#include "app/hello_world/hello_world.h"

#include "test/test.h"

static int log_level = 0;
static char *env_file = NULL;
static char *app_name = NULL;
static volatile int running = 1;

static void print_usage(void)
{
    fprintf(stderr,
            "Usage: %s [<options>] [<arguments>]\n\n"
            "Options:\n"
            "\t-c <path>  set the search path for config files (default: /data/hello-world/.env)\n"
            "\t-l <str>   set log_level {fatal, error, warn, info, debug}\n"
            "\t-v         app version \n"
            "\t-h         help\n"
            "\n",
            app_name);
}

static void parse_option(int argc, char **argv)
{
    int c;
    FILE *fp;
    while ((c = getopt(argc, argv, "c:l:hv")) != -1)
    {
        switch (c)
        {
        case 'c':
            fp = fopen(optarg, "r");
            if (fp != NULL)
            {
                env_file = optarg;
            }
            break;
        case 'l':
            log_level = log_level_number(optarg);
            if (log_level < 0)
            {
                fprintf(stderr, "Invalid Log Param\n");
                exit(1);
            }
            break;
        case 'v':
            fprintf(stderr, "Version : %s\n", VERSION);
            exit(0);
            break;
        case 'h':
            print_usage();
            exit(0);
            break;
        default:
            fprintf(stderr, "Invalid Option: %c\n", c);
            print_usage();
            exit(1);
            break;
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "Invalid Argument: ");
        while (optind < argc)
        {
            fprintf(stderr, "%s  ", argv[optind++]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }
}

/**
 * @brief Initialize
 *
 */
void init()
{
    log_init(app_name, log_level);
    env_init(env_file);

    /* sys core */
    timer_init();
    go_init();

    /* application */
    // app_init();

    /* testing */
    // test_init();

    // sleep(3);
    // exit(0); // TODO: test only
}

/**
 * @brief Finalize
 *
 */
void final()
{
    log_final();
}

/**
 * @brief
 *
 * @param dm
 */
void signal_handler(int dm)
{
    running = 0;
}

/**
 * @brief loop until signal interrupt
 *
 */
void wait_ctrlC()
{
    signal(SIGINT, signal_handler);
    while (running)
    {
        sleep(1);
    }
    log_info("SIGINT");
}

int main(int argc, char *argv[])
{
    app_name = argv[0];
    parse_option(argc, argv);

    log_intro();

    init();

    app_hello_world();

    wait_ctrlC();

    final();

    log_debug("EXIT");
    return 0;
}
