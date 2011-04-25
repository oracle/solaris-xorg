/*
 * Copyright (c) 2006, 2009, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <sys/param.h>		/* MAXPATHLEN */
#include <errno.h>
#include <pwd.h>		/* getpwuid() */
#include <signal.h>
#include <stdio.h>		/* snprintf() */
#include <stdlib.h>		/* exit(), malloc() */
#include <string.h>		/* strcat(), strcpy() */
#include <unistd.h>		/* sleep() */
#include <sys/fbio.h>
#include <sys/mman.h>
#include <sys/systeminfo.h>	/* sysinfo() */
#include <sys/visual_io.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "graphicstest.h"
#include "gfx_vts.h"		/* VTS Graphics Test common routines */

#include "libvtsSUNWast.h"	/* VTS library definitions for ast device */


#if (0)	/* Unused */
#define	NO_DMA			0
#define	USE_DMA			1
#endif

#define	MAX_DISPLAY_LEN		261


static int		no_window = 1;
static gfxtest_info	*tests_info;
static int		received_control_c = 0;
static int		need_screen_lock = 1;
static Display		*dpy = NULL;


/* Declarations needed for get_tests() */

static unsigned int ast_mask_list[] = {
	GRAPHICS_TEST_OPEN,
	GRAPHICS_TEST_DMA,
	GRAPHICS_TEST_MEM,
	GRAPHICS_TEST_CHIP
};

static unsigned int ast_mesg_list[] = {
	GRAPHICS_TEST_OPEN_MESG,
	GRAPHICS_TEST_DMA_MESG,
	GRAPHICS_TEST_MEM_MESG,
	GRAPHICS_TEST_CHIP_MESG
};

static gfxtest_function ast_test_list[] = {
	ast_test_open,
	ast_test_dma,
	ast_test_memory,
	ast_test_chip
};


static void
disable_pm(Display *dpy)
{
	int		dummy;

	XSetScreenSaver(dpy, 0, 0, 0, 0);
	if (DPMSQueryExtension(dpy, &dummy, &dummy)) {
	    DPMSDisable(dpy);
	}
	if (FBPMQueryExtension(dpy, &dummy, &dummy)) {
	    FBPMDisable(dpy);
	}

}	/* disable_pm() */


int
lock_display(int fd, Display** current_display)
{
	char		env_buf[5 + MAXPATHLEN]; /* "HOME=<pw_dir>" */
	int		i;
	int		screen;
	Display		*dpy;
	Window		win;
	XSetWindowAttributes xswa;
	char		hostname[MAX_DISPLAY_LEN];
	char		display[MAX_DISPLAY_LEN];
	struct sigaction act;
	int		current_screen;
	struct hostent	*Host;
	struct passwd	*pw_entry;
	int		status;
	char		no_bits[] = { 0 };
	XColor		dumcolor;
	Pixmap		lockc;
	Pixmap		lockm;
	Cursor		cursor;

	pw_entry = getpwuid(0);
	if (strlen(pw_entry->pw_dir) >= MAXPATHLEN) {
	    TraceMessage(VTS_DEBUG, __func__,
			    "HOME= directory path is too long\n");
	    return (1);
	}
	strcpy(env_buf, "HOME=");
	strcat(env_buf, pw_entry->pw_dir);
	if (putenv(env_buf) != 0) {
	    TraceMessage(VTS_DEBUG, __func__,
			    "putenv( HOME= ) failed, errno:%d\n", errno);
	    return (1);
	}

	dpy = NULL;

	if (gfx_vts_debug_mask & GRAPHICS_VTS_SLOCK_OFF) {
	    TraceMessage(VTS_DEBUG, __func__, "lock_display() DISABLED\n");
	    need_screen_lock = 0;
	    *current_display = NULL;
	    return (0);
	}

	current_screen = 0;

	TraceMessage(VTS_DEBUG, __func__, "locking X screen %d\n",
		    current_screen);

#if (0)
	/* Get the host machine name */
	if (sysinfo(SI_HOSTNAME, hostname, MAX_DISPLAY_LEN) == -1) {
	    TraceMessage(VTS_DEBUG, __func__,
			"sysinfo(2) failed getting hostname\n");
	    hostname[0] = '\0';
	}
#else
	hostname[0] = '\0';
#endif

	snprintf(display, sizeof (display), "%s:0.%d",
		hostname, current_screen);
	dpy = XOpenDisplay(display);
	TraceMessage(VTS_DEBUG, __func__,
		    "XOpenDisplay, display = %s, dpy = 0x%p\n", display, dpy);

	if (dpy == NULL) {
	    TraceMessage(VTS_DEBUG, __func__, "Assuming no window_system\n");
	    return (0);
	}

	TraceMessage(VTS_DEBUG, __func__,
		    "XOpenDisplay successful, display = %s, dpy = 0x%p\n",
		    display, dpy);

	screen = DefaultScreen(dpy);

	/*
	 * Flush request buffer and wait for all requests to be processed
	 */
	XSync(dpy, False);

	/* Tell server to report events as they occur */
	XSynchronize(dpy, True);

	disable_pm(dpy);

	/* Create a blank cursor */
	lockc = XCreateBitmapFromData(dpy, RootWindow(dpy, 0),
					no_bits, 1, 1);
	lockm = XCreateBitmapFromData(dpy, RootWindow(dpy, 0),
					no_bits, 1, 1);
	cursor = XCreatePixmapCursor(dpy, lockc, lockm, &dumcolor,
					&dumcolor, 0, 0);

	XFreePixmap(dpy, lockc);
	XFreePixmap(dpy, lockm);

	xswa.cursor = cursor;
	xswa.override_redirect = True;
	xswa.event_mask = (KeyPressMask | KeyReleaseMask | ExposureMask);
	no_window = 0;
	win = XCreateWindow(dpy,
			    RootWindow(dpy, screen),
			    0, 0,
			    DisplayWidth(dpy, current_screen),
			    DisplayHeight(dpy, current_screen),
			    0,
			    CopyFromParent,
			    InputOutput,
			    CopyFromParent,
			    CWOverrideRedirect | CWEventMask, &xswa);

	TraceMessage(VTS_DEBUG, __func__, " XCreateWindow win=%d\n", win);

	XMapWindow(dpy, win);
	XRaiseWindow(dpy, win);
	TraceMessage(VTS_DEBUG, __func__, " no_window=%d\n", no_window);

	if (!no_window) {
	    /* Disable server from handling any requests */
	    XGrabServer(dpy);
	    /* Gain control of keyboard */
	    status = XGrabKeyboard(dpy, win, False, GrabModeAsync,
				   GrabModeAsync, CurrentTime);

	    if (status != GrabSuccess) {
		TraceMessage(VTS_DEBUG, __func__,
			    "Cannot gain control of keyboard\n");
	    }

	    status = XGrabPointer(dpy,
				win,
				False,
				ResizeRedirectMask,
				GrabModeAsync,
				GrabModeAsync,
				None,
				cursor,
				CurrentTime);
	    if (status != GrabSuccess) {
		TraceMessage(VTS_DEBUG, __func__,
			    "Cannot gain control of pointer\n");
	    }
	}

	sleep(2);
	*current_display = dpy;
	return (0);

}	/* lock_display() */

void
unlock_display(Display *dpy)
{
	if (dpy) {
		XUngrabPointer(dpy, CurrentTime);
		XUngrabKeyboard(dpy, CurrentTime);
		XUngrabServer(dpy);
	}
}


/* *** PUBLIC *** */

/* These library functions are public and are expected to exist */

int
get_tests(gfxtest_info *tests)
{
	return_packet *ast_test_open(int fd);

	/*
	 * Set the gfx_vts_debug_mask bits according to environment variables
	 */
	gfx_vts_set_debug_mask();

	/*
	 * Disable screen lock by default
	 */
	gfx_vts_debug_mask |= GRAPHICS_VTS_SLOCK_OFF;

	/*
	 * Construct the list of tests to be performed
	 */
	tests->count = sizeof (ast_test_list) / sizeof (gfxtest_function);
	tests->this_test_mask = (int *)malloc(sizeof (ast_mask_list));
	tests->this_test_mesg = (int *)malloc(sizeof (ast_mesg_list));
	tests->this_test_function =
			(gfxtest_function *)malloc(sizeof (ast_test_list));

	if ((tests->this_test_mask     == NULL) ||
	    (tests->this_test_mesg     == NULL) ||
	    (tests->this_test_function == NULL)) {
	    gfx_vts_free_tests(tests);
	    return (GRAPHICS_ERR_MALLOC_FAIL);
	}

	tests->connection_test_function = ast_test_open;

	memcpy(tests->this_test_mask, ast_mask_list, sizeof (ast_mask_list));
	memcpy(tests->this_test_mesg, ast_mesg_list, sizeof (ast_mesg_list));
	memcpy(tests->this_test_function, ast_test_list,
						sizeof (ast_test_list));

	tests_info = tests;
	return (0);

}	/* get_tests() */


int
cleanup_tests(gfxtest_info *tests)
{

	TraceMessage(VTS_DEBUG, __func__, "call cleanup_tests\n");
	gfx_vts_free_tests(tests);

	if (need_screen_lock) {
	    unlock_display(dpy);
	}

}	/* cleanup_tests() */


/*
 * ast_test_open()
 *
 *    This test will open the device, read and write some registers
 *    after mmaping in the register and frame buffer spaces.
 */

return_packet *
ast_test_open(int fd)
{
	static return_packet rp;
	int		rc = 0;
	struct vis_identifier vis_identifier;

	if (need_screen_lock) {
	    lock_display(fd, &dpy);
	}

	/* setup */
	memset(&rp, 0, sizeof (return_packet));

	if (gfx_vts_check_fd(fd, &rp)) {
	    return (&rp);
	}

	TraceMessage(VTS_TEST_STATUS, __func__, "check_fd passed.\n");

	/* vis identifier will do this */
	rc = ioctl(fd, VIS_GETIDENTIFIER, &vis_identifier);

	TraceMessage(VTS_TEST_STATUS, __func__, "rc = %d\n", rc);

	if (rc != 0) {
	    gfx_vts_set_message(&rp, 1, GRAPHICS_ERR_OPEN, NULL);
	    return (&rp);
	}

	if (strncmp(vis_identifier.name, "SUNWast", 7) != 0) {
	    gfx_vts_set_message(&rp, 1, GRAPHICS_ERR_OPEN, NULL);
	    return (&rp);
	}

	map_me(&rp, fd);

	TraceMessage(VTS_DEBUG, __func__, "Open completed OK\n");

	check4abort(dpy);
	return (&rp);

}	/* ast_test_open() */


/*
 * ast_test_dma
 *
 *    This test will open the device, allocate the dma buffers to
 *    separate memory spaces, and read/write the data, verifying it.
 */

return_packet *
ast_test_dma(int fd)
{
	static return_packet rp;
	int		i;

	TraceMessage(VTS_DEBUG, __func__, "ast_test_dma running\n");

	if (need_screen_lock) {
	    lock_display(fd, &dpy);
	}

	dma_test(&rp, fd);

	TraceMessage(VTS_DEBUG, __func__, " ast_test_dma completed\n");

	check4abort(dpy);
	return (&rp);

}	/* ast_test_dma() */


/*
 * ast_test_memory()
 *
 *    This test will open the device and read and write to all memory
 *    addresses.
 */

return_packet *
ast_test_memory(int fd)
{
	static return_packet rp;

	TraceMessage(VTS_DEBUG, __func__, " ast_test_memory running\n");

	if (gfx_vts_debug_mask & GRAPHICS_VTS_MEM_OFF) {
	    return (&rp);
	}

	if (need_screen_lock) {
	    lock_display(fd, &dpy);
	}

	memory_test(&rp, fd);

	TraceMessage(VTS_DEBUG, __func__, " ast_test_memory completed\n");

	check4abort(dpy);
	return (&rp);

}	/* ast_test_memory() */


/*
 * ast_test_chip()
 *
 *    Test Chip, functional tests.
 */

return_packet *
ast_test_chip(int fd)
{
	static return_packet rp;

	if (gfx_vts_debug_mask & GRAPHICS_VTS_CHIP_OFF) {
	    return (&rp);
	}
	TraceMessage(VTS_DEBUG, __func__, " ast_test_chip running\n");

	if (need_screen_lock) {
	    lock_display(fd, &dpy);
	}

	chip_test(&rp, fd);

	TraceMessage(VTS_DEBUG, __func__, " ast_test_chip completed\n");

	check4abort(dpy);
	return (&rp);

}	/* ast_test_chip() */


void
graphicstest_finish(int flag)
{

	TraceMessage(VTS_DEBUG, __func__, "call graphicstest_finish\n");

	TraceMessage(VTS_DEBUG, __func__, "call reset_memory_state\n");

	cleanup_tests(tests_info);

	exit(0);

}	/* graphicstest_finish() */


/*
 * check4abort()
 *
 *    This function sends a KILL signal to the program if it detects
 *    that the user has pressed ^C.  This functionality is usually
 *    performed by the Command Tool which spawned a program, but in this
 *    case we need to do it because we have grabbed all keyboard events.
 *    It should be called anywhere where it's safe to end the program.
 */

void
check4abort(Display *dpy)
{
#define	CTRL_C	'\003'			/* Ctrl-C (^C) */

	/*
	 * If necessary, restore the original state following a test
	 */
#if !defined(VTS_STUBS)
	chip_test_reset();
#endif

	if (dpy != NULL) {
#if !defined(VTS_STUBS)
	    while (XPending(dpy)) {
		XEvent	event;		/* Key event structure */
		int	i;		/* Loop counter / keystr[] index */
		char	keystr[5] = "";	/* Buffer for returned string */
		int	len;		/* Length of returned string */

		XNextEvent(dpy, &event);
		if (event.type == KeyPress) {
		    len = XLookupString((XKeyEvent *)&event,
					keystr, sizeof (keystr),
					NULL, NULL);
		    for (i = 0; i < len; i++) {
			if (keystr[i] == CTRL_C) {
			    kill(getpid(), SIGINT);
			    graphicstest_finish(0);
			}
		    }
		}
	    }
#endif	/* VTS_STUBS */
	}

}	/* check4abort() */


/* End of libvtsSUNWast.c */
