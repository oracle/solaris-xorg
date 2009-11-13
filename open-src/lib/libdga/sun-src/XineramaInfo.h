/* Copyright 1999 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)XineramaInfo.h	1.3	09/11/09 SMI"

#define XinID	int
#define MAXSCREEN 16
#define DELTA	int
#define POINT	int
typedef struct subwid
{
	XinID	wid;	/* sub window id */
	DELTA	dx,dy;	/* delta in screen co-ord from virtual zero */
	POINT	x,y;	/* location of window in screen co-ord */
	DELTA 	wdx,wdy;/* size of window in screen co-ord */
}SubWID, *pSubWID;

typedef struct xineramainfo
{
	XinID 	wid;	/* Window ID of requested virtual window */
	SubWID	subs[MAXSCREEN];	/* there will be 16 slots */
}XineramaInfo, *pXineramaInfo;

typedef struct _XDgaXineramaInfoReply
{
	BYTE	type;
	CARD8	unused;
	CARD16	sequenceNumber B16;
	CARD32	length B32;
	XinID 	wid;	/* Window ID of requested virtual window */
	SubWID	subs[MAXSCREEN];	/* there will be 16 slots */
}xDgaXineramaInfoReply;

typedef struct _XDgaXineramaInfoReq
{
	CARD8	reqType;
	CARD8	xdgaReqType;
	CARD16	length B16;
	CARD32	visual B32;
}xDgaXineramaInfoReq;
#define sz_xDgaXineramaInfoReq 8

