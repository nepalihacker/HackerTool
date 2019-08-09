#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h"
#include "ikeylog.h"
#include "args.h"


/**
 * This function handles select signals that the process may
 * receive. This gives us the chance to handle exits properly. 
 * 
 * @param sig The signal received
 */
void signal_handler(int sig) {
    if(g_ixkeylog_opts->debug > 0)
        M_DEBUG_INFO("caught signal: %d\n", sig);
        
    /* Specify here how to handle signals */
    switch(sig) {
        case SIGHUP:
            break;                          /* TODO: Signal handler */
        case SIGTERM:
            break;                          /* TODO: Signal handler */          
        case SIGINT:
            break;                          /* TODO: Signal handler */
        default:
            break;
    }
}
