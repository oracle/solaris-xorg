/*-
 * XCrDynCmap.c - X11 library routine to create dynamic colormaps.
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


#include <X11/X.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Status
XCreateDynamicColormap(dsp, screen, cmap, visual, colors,
		       count, red, green, blue)
    Display    *dsp;
    int		screen;
    Colormap   *cmap;		/* return */
    Visual    **visual;		/* return */
    XColor     *colors;
    int 	count;
    u_char     *red,
	       *green,
	       *blue;
{
    XVisualInfo vinfo;
    int		pixels[256];
    int		i,
		ncolors,
		planes;
    unsigned long pmasks;
    Status	allocReturn;

    planes = DisplayPlanes(dsp, screen);
    if (!XMatchVisualInfo(dsp, screen, planes, PseudoColor, &vinfo)) 
        if (!XMatchVisualInfo(dsp, screen, planes, GrayScale, &vinfo)) 
            if (!XMatchVisualInfo(dsp, screen, planes, DirectColor, &vinfo)) 
	        return BadMatch;

    {
	*visual = vinfo.visual;
	*cmap = XCreateColormap(dsp, RootWindow(dsp, screen),
				*visual, AllocNone);
	ncolors = vinfo.colormap_size;

	if (count > ncolors)
	    return BadValue;

	allocReturn = XAllocColorCells(dsp, *cmap,
				       False, &pmasks, 0,
				       pixels, ncolors);

/*	This should return Success, but it doesn't... Xlib bug?
 *	(I'll ignore the return value for now...)
 */
#ifdef NOTDEF
	if (allocReturn != Success)
	    return allocReturn;
#endif				/* NOTDEF */

	for (i = 0; i < ncolors - count; i++) {
	    colors[i].pixel = pixels[i];
	    XQueryColor(dsp, DefaultColormap(dsp, screen), &colors[i]);
	}
	for (i = ncolors - count; i < ncolors; i++) {
	    colors[i].pixel = pixels[i];
	    colors[i].red = *red++ << 8;
	    colors[i].green = *green++ << 8;
	    colors[i].blue = *blue++ << 8;
	    colors[i].flags = DoRed | DoGreen | DoBlue;
	}
	XStoreColors(dsp, *cmap, colors, ncolors);
	return Success;
    }
}
