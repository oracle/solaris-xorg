/*
 * Copyright 1999 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
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
 * 
 */

#ifndef _HWCIO_H
#define	_HWCIO_H


#ifdef	__cplusplus
extern "C" {
#endif

/*
 * These ioctl numbers have not been allocated and approved by USL.
 */
#define HIOC          ('H' << 8)

/* "control" ioctls (i.e. only valid on unit 0) */
#define HWCSPOS		(HIOC|1)	/* Set Cursor Position */
#define HWCSCURSOR	(HIOC|2)	/* Set Cursor Image */
#define HWCSSPEED	(HIOC|3)	/* Set Cursor Acceleration */
#define HWCSSCREEN	(HIOC|4)	/* Set Framebuffer */
#define HWCSBOUND	(HIOC|5)	/* Set Cursor Boundary */
#define HWCGVERSION	(HIOC|6)	/* Get HWC version */
#define HWCENABLE	(HIOC|7)	/* Enable cursor */

/*
 * Version 3 supports constraining cursor to a window and return a motion
 * event on a HWCSPOS ioctl.
 *
 * Version 4 supports all version 3 features but changes the HWCSSCREEN
 * ioctl to use more stable ON consolidation private interfaces.
 *
 * Version 5 adds the HWCENABLE ioctl.  When we went to the version 4
 * interface, a value of -1 can no longer be used to signify disable
 * because dev_t is a ulong_t.  This wasn't caught until after version 4
 * had made it into a build so we bump the version number.
 */
#define	HWCVERSION	5

#define GETFBCURSOR             0
#define GETIMAGE                1
#define GETMASK                 2
#define GETCMAPRED              3
#define GETCMAPGREEN            4
#define GETCMAPBLUE             5

struct hwc_state {
        int             st_state;
        struct fbcursor fbcursor;
        int		count;
        char            *image;
        char            *mask;
        unsigned char   *cmap_red;
        unsigned char   *cmap_green;
        unsigned char   *cmap_blue;
};

#if defined(_SYSCALL32_IMPL)

struct hwc_state32 {
	int32_t		st_state;
	struct fbcursor32 fbcursor;
	int32_t		count;
	caddr32_t	image;
	caddr32_t	mask;
	caddr32_t	map_red;
	caddr32_t	cmap_green;
	caddr32_t	cmap_blue;
};
#endif /* _SYSCALL32_IMPL */

/* NOTE: datamodel size-invariant ILP32/LP64 */
struct hwc_limits {
	int 	confined;
	short 	x1, y1, x2, y2;
};

#ifdef	__cplusplus
}
#endif

#endif  /* _HWCIO_H */

