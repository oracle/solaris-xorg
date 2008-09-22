/*
 * Copyright (c) 1988-91 by Patrick J. Naughton.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 */

/*
 * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 */

/*-
 * @(#)xlock.h	35.8 08/09/18 XLOCK
 *
 * xlock.h - external interfaces for new modes and SYSV OS defines.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>

#define MAXSCREENS 16
#define NUMCOLORS 64

typedef struct {
    GC          gc;		/* graphics context for animation */
    int         npixels;	/* number of valid entries in pixels */
    u_long      pixels[NUMCOLORS];	/* pixel values in the colormap */
}           perscreen;

extern perscreen Scr[MAXSCREENS];
extern Display *dsp;
extern int  screen;

extern char *ProgramName;
extern char *display;
extern char *mode;
extern char *fontname;
extern char *background;
extern char *foreground;
extern char *text_name;
extern char *text_pass;
extern char *text_info;
extern char *text_valid;
extern char *text_invalid;
extern float saturation;
extern int  nicelevel;
extern int  delay;
extern int  batchcount;
extern int  reinittime;
extern int  timeout;
extern Bool usefirst;
extern Bool mono;
extern Bool nolock;
extern Bool allowroot;
extern Bool enablesaver;
extern Bool allowaccess;
extern Bool echokeys;
extern Bool verbose;
extern void (*callback) ();
extern void (*init) ();

extern void GetResources();
extern void hsbramp();
extern void error();
extern long seconds();
extern void usage();

/* System V Release 4 redefinitions of BSD functions and structures */

#ifdef SYSV

#include <sys/time.h>
#include <poll.h>
#include <shadow.h>
/*
#define srandom srand
#define random rand
#define MAXRAND (32767.0)
*/
#define MAXRAND (2147483648.0)

#else

#define MAXRAND (2147483648.0)

#endif
