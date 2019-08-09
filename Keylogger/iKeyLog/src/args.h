#ifndef __ARGS_H__
#define __ARGS_H__

#include <unistd.h>
#include <getopt.h>


/* Useful variables */
static int args_debug_flag             = 0;
static int args_daemonize_flag         = 0;
static int args_version_flag           = 0;


/**
 * Long options for getopts
 */
static const struct option long_options[] =
{
    /* These arguments set flags */
    {"debug",       0,    &args_debug_flag,     1},
    {"daemon",      0,    &args_daemonize_flag, 1},
    {"version",     0,    &args_version_flag,   1},
    
    /* These ones require arguments */
    {"output",  1, 0, 'o'},
    {"display", 1, 0, 'x'},
    {0, 0, 0, 0}
};

/** 
 * Own options structures
 */
typedef struct _opts {
    const char *display;        /** Where to log */
    const char *output;         /** Where to dump logged data */
    int daemon;                 /** Daemonize process? */
    int debug;                  /** Show debug information */
    int version;
} ikeylog_opts;


/* Function declarations */
ikeylog_opts *args_check(int, char **);


/* Global stuff */
ikeylog_opts *g_ikeylog_opts;


#endif /* args.h */
