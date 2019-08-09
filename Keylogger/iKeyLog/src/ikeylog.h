#ifndef __IKEYLOG_H__
#define __IKEYLOG_H__

#include "display.h"

#define D_IKEYLOG_VERSION    __GIT_VERSION
#define D_IKEYLOG_AUTHOR     "Roshan Lamichhane"


/**
 * Useful macros
 */
#define M_DISPLAY_ERROR fprintf(stderr, ">> Error: %s\n", strerror(errno))
#define M_DEBUG_INFO(format, args...) \
        fprintf(stdout, "[DEBUG] (" __FILE__ ") " format, args);
        
#define M_ERROR(e)      { M_DISPLAY_ERROR; exit(e); }


#endif /* ikeylog.h */
