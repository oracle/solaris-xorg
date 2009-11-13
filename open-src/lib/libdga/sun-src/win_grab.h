/* Copyright 1993 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)win_grab.h	35.2	09/11/09 SMI"

/*
 * win_grab.h - DGA Window Grabber definitions
 */

#ifndef _WIN_GRAB_H
#define _WIN_GRAB_H


/****
 *
 * WIN_GRAB.H Window Grabber
 *
 * MACROS: These return info from the client page.
 *
 *	wx_infop(clientp)	the info page.
 *	wx_devname_c(clientp)	returns the w_devname field.
 *	wx_version_c(clientp)	the version
 *	wx_lock_c(clientp)	locks the info page.
 *	wx_unlock_c(clientp)	unlocks the info page.
 *
 * These return info from the info page.
 *
 *	wx_devname(infop)	returns the w_devname field.
 *	wx_version(infop)	the version
 *
 *	wx_shape_flags(infop)	Shapes flag
 *
 ****/



#define	DGA_SH_RECT_FLAG	1
#define	DGA_SH_EMPTY_FLAG	128


/*
 *  Window grabber info macros
 */

#define	wx_infop(clientp)	((WXINFO *) (clientp)->w_info)

#define	wx_version(infop)	((infop)->w_version)

#define wx_shape_flags(infop)			\
	(((struct class_SHAPE_vn *)		\
	    ((char *)(infop) +			\
	     (infop)->u.vn.w_shapeoff))->SHAPE_FLAGS)	\


#endif /* _WIN_GRAB_H */
