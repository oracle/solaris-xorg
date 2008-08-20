/*-
 * XCrHsbCmap.c - X11 library routine to create an HSB ramp colormaps.
 *
 * Copyright 1995 Sun Microsystems, Inc.  All rights reserved.
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
 *
 * Author: Patrick J. Naughton
 * naughton@sun.com
 */
#pragma ident "@(#)XCrHsbCmap.c	35.3 08/08/21 XCOLOR SMI"

#include <X11/X.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>

extern void HSBmap();

Status
XCreateHSBColormap(dsp, screen, cmap, count, h1, s1, b1, h2, s2, b2, bw, visual)
    Display    *dsp;
    int         screen;
    Colormap   *cmap;		/* colormap return value */
    int         count;		/* number of entrys to use */
    double      h1,		/* starting hue */
                s1,		/* starting saturation */
                b1,		/* starting brightness */
                h2,		/* ending hue */
                s2,		/* ending saturation */
                b2;		/* ending brightness */
    int         bw;		/* Boolean: True = save black and white */
    Visual    **visual;
{
    u_char      red[256];
    u_char      green[256];
    u_char      blue[256];
    unsigned long pixel;
    Status      status;
    XColor      xcolors[256];

    if (count > 256)
	return BadValue;

    HSBramp(h1, s1, b1, h2, s2, b2, 0, count - 1, red, green, blue);

    if (bw) {
	pixel = WhitePixel(dsp, screen);
	if (pixel < 256)
	    red[pixel] = green[pixel] = blue[pixel] = 0xff;

	pixel = BlackPixel(dsp, screen);
	if (pixel < 256)
	    red[pixel] = green[pixel] = blue[pixel] = 0;
    }
    status = XCreateDynamicColormap(dsp, screen, cmap, visual, xcolors,
				    count, red, green, blue);

    return status;
}
