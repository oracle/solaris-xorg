/* Copyright 1990 Sun Microsystems, Inc.  All rights reserved.
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


#ifndef _ALLPLANESSTR_H_
#define _ALLPLANESSTR_H_

#include "allplanes.h"

#define ALLPLANESNAME "SUN_ALLPLANES"
#define ALLPLANES_MAJOR_VERSION	1	/* current version numbers */
#define ALLPLANES_MINOR_VERSION	0

typedef struct {
    CARD8       reqType;		/* always AllPlanesReqCode */
    CARD8       allplanesReqType;	/* always X_AllPlanesQueryVersion */
    CARD16	length B16;
} xAllPlanesQueryVersionReq;
#define sz_xAllPlanesQueryVersionReq	4

typedef struct {
    BYTE        type;			/* X_Reply */
    CARD8       unused;
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD16	majorVersion B16;
    CARD16	minorVersion B16;
    CARD32	pad0 B32;
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
} xAllPlanesQueryVersionReply;
#define sz_xAllPlanesQueryVersionReply	32

typedef struct {
    CARD8       reqType;		/* always AllPlanesReqCode */
    CARD8       allplanesReqType;	/* always X_AllPlanesPolySegment */
    CARD16	length B16;
    Drawable	drawable B32;
} xAllPlanesPolySegmentReq;
#define sz_xAllPlanesPolySegmentReq 8

typedef xAllPlanesPolySegmentReq xAllPlanesPolyRectangleReq;
#define sz_xAllPlanesPolyRectangleReq 8

typedef xAllPlanesPolySegmentReq xAllPlanesPolyFillRectangleReq;
#define sz_xAllPlanesPolyFillRectangleReq 8

typedef struct {
    CARD8       reqType;		/* always AllPlanesReqCode */
    CARD8       allplanesReqType;	/* always X_AllPlanesPolyPoint */
    CARD16	length B16;
    Drawable	drawable B32;
    BYTE	coordMode;
    CARD8	pad0;
    CARD16	pad1 B16;
} xAllPlanesPolyPointReq;
#define sz_xAllPlanesPolyPointReq 12

typedef xAllPlanesPolyPointReq xAllPlanesPolyLineReq;
#define sz_xAllPlanesPolyLineReq 12
#endif /* _ALLPLANESSTR_H_ */
