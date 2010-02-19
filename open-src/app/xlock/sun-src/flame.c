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
 * Copyright 1994 Sun Microsystems, Inc.  All rights reserved.
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
 * flame.c - recursive fractal cosmic dust.
 *
 * Copyright (c) 1991 by Patrick J. Naughton.
 *
 * See xlock.c for copying information.
 *
 * Revision History:
 * 24-Jun-91: fixed portability problem with integer mod (%).
 * 06-Jun-91: Written. (received from Spot, spot@cs.cmu.edu).
 */

#include "xlock.h"
#include <math.h>

#define MAXTOTAL	10000
#define MAXBATCH	10

typedef struct {
    double      f[2][3][20];	/* three non-homogeneous transforms */
    int         max_levels;
    int         cur_level;
    int         SNUM;
    int         ANUM;
    int         width, height;
    int         num_points;
    int         total_points;
    int         pixcol;
    Window      win;
    XPoint      pts[MAXBATCH];
}           flamestruct;

extern XColor ssblack[];
extern XColor sswhite[];

static flamestruct flames[MAXSCREENS];

static short
halfrandom(mv)
    int         mv;
{
    static short lasthalf = 0;
    unsigned long r;

    if (lasthalf) {
	r = lasthalf;
	lasthalf = 0;
    } else {
	r = random();
	lasthalf = r >> 16;
    }
    return r % mv;
}

void
initflame(win)
    Window      win;
{
    flamestruct *fs = &flames[screen];
    XWindowAttributes xwa;

    srandom(time((long *) 0));

    XGetWindowAttributes(dsp, win, &xwa);
    fs->width = xwa.width;
    fs->height = xwa.height;

    fs->max_levels = batchcount;
    fs->win = win;

    XSetForeground(dsp, Scr[screen].gc, ssblack[screen].pixel);
    XFillRectangle(dsp, win, Scr[screen].gc, 0, 0, fs->width, fs->height);

    if (Scr[screen].npixels > 2) {
	fs->pixcol = halfrandom(Scr[screen].npixels);
	XSetForeground(dsp, Scr[screen].gc, Scr[screen].pixels[fs->pixcol]);
    } else {
	XSetForeground(dsp, Scr[screen].gc, sswhite[screen].pixel);
    }
}

static      Bool
recurse(fs, x, y, l)
    flamestruct *fs;
    register double x, y;
    register int l;
{
    int         xp, yp, i;
    double      nx, ny;

    if (l == fs->max_levels) {
	fs->total_points++;
	if (fs->total_points > MAXTOTAL)	/* how long each fractal runs */
	    return False;

	if (x > -1.0 && x < 1.0 && y > -1.0 && y < 1.0) {
	    xp = fs->pts[fs->num_points].x = (int) ((fs->width / 2)
						    * (x + 1.0));
	    yp = fs->pts[fs->num_points].y = (int) ((fs->height / 2)
						    * (y + 1.0));
	    fs->num_points++;
	    if (fs->num_points > MAXBATCH) {	/* point buffer size */
		XDrawPoints(dsp, fs->win, Scr[screen].gc, fs->pts,
			    fs->num_points, CoordModeOrigin);
		fs->num_points = 0;
	    }
	}
    } else {
	for (i = 0; i < fs->SNUM; i++) {
	    nx = fs->f[0][0][i] * x + fs->f[0][1][i] * y + fs->f[0][2][i];
	    ny = fs->f[1][0][i] * x + fs->f[1][1][i] * y + fs->f[1][2][i];
	    if (i < fs->ANUM) {
		nx = sin(nx);
		ny = sin(ny);
	    }
	    if (!recurse(fs, nx, ny, l + 1))
		return False;
	}
    }
    return True;
}


void
drawflame(win)
    Window      win;
{
    flamestruct *fs = &flames[screen];

    int         i, j, k;
    static      alt = 0;

    if (!(fs->cur_level++ % fs->max_levels)) {
	XClearWindow(dsp, fs->win);
	alt = !alt;
    } else {
 	if (Scr[screen].npixels > 2) {
  	    XSetForeground(dsp, Scr[screen].gc,
 			   Scr[screen].pixels[fs->pixcol]);
	    if (--fs->pixcol < 0)
 		fs->pixcol = Scr[screen].npixels - 1;
 	}
    }

    /* number of functions */
    fs->SNUM = 2 + (fs->cur_level % 3);

    /* how many of them are of alternate form */
    if (alt)
	fs->ANUM = 0;
    else
	fs->ANUM = halfrandom(fs->SNUM) + 2;

    /* 6 coefs per function */
    for (k = 0; k < fs->SNUM; k++) {
	for (i = 0; i < 2; i++)
	    for (j = 0; j < 3; j++)
		fs->f[i][j][k] = ((double) (random() & 1023) / 512.0 - 1.0);
    }
    fs->num_points = 0;
    fs->total_points = 0;
    (void) recurse(fs, 0.0, 0.0, 0);
    XDrawPoints(dsp, win, Scr[screen].gc,
		fs->pts, fs->num_points, CoordModeOrigin);
}
