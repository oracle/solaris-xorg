/*
 * Copyright 1999 Sun Microsystems, Inc.  All rights reserved.
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
 */

#ifndef _HWCIO_H
#define	_HWCIO_H

#pragma ident	"@(#)hwcio.h	35.6	08/08/12 SMI"

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

