/* Copyright 1994 Sun Microsystems, Inc.  All rights reserved.
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


#ifndef	_DGA_EXTERNALDEFS_H
#define	_DGA_EXTERNALDEFS_H

/*
** External defines for libdga.
*/

#define DGA_DRAW_WINDOW			0
#define DGA_DRAW_PIXMAP			1
#define DGA_DRAW_OVERLAY		2

#define DGA_MBACCESS_NONE		0
#define DGA_MBACCESS_SINGLEADDR		1
#define DGA_MBACCESS_MULTIADDR		2

#define DGA_SITE_NULL			0
#define DGA_SITE_DEVICE			1
#define DGA_SITE_SYSTEM			2

#define DGA_SITECHG_UNKNOWN		0
#define DGA_SITECHG_INITIAL		1
#define DGA_SITECHG_ZOMBIE		2
#define DGA_SITECHG_ALIAS		3
#define DGA_SITECHG_CACHE		4
#define DGA_SITECHG_MB			5

#define DGA_MBCHG_UNKNOWN		0
#define DGA_MBCHG_ACTIVATION		1
#define DGA_MBCHG_DEACTIVATION		2
#define DGA_MBCHG_REPLACEMENT	        3

#define DGA_VIS_UNOBSCURED		0
#define DGA_VIS_PARTIALLY_OBSCURED	1
#define DGA_VIS_FULLY_OBSCURED		2

#define DGA_OVLSTATE_SAFE		0
#define DGA_OVLSTATE_MULTIWID		1
#define DGA_OVLSTATE_CONFLICT		2

#define DGA_MAX_GRABBABLE_BUFS       16

#endif /* _DGA_EXTERNALDEFS_H */
