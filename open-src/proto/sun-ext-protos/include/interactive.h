/*
 * Copyright 1994 Sun Microsystems, Inc.  All rights reserved.
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

/************************************************************
	Protocol defs for XIA extension
********************************************************/

/* THIS IS NOT AN X CONSORTIUM STANDARD */

#ifndef _INTERACTIVE_H
#define _INTERACTIVE_H


#include <X11/Xproto.h>

#include "interactiveCommon.h"

#define X_IAQueryVersion                0
#define X_IASetProcessInfo              1
#define X_IAGetProcessInfo              2

#define IANumberEvents                  0
#define IANumberErrors                  0

typedef int ConnectionPidRec;
typedef int * ConnectionPidPtr;

#define IANAME "SolarisIA"

#define IA_MAJOR_VERSION	1	/* current version numbers */
#define IA_MINOR_VERSION	1

typedef struct _IAQueryVersion {
    CARD8	reqType;		/* always IAReqCode */
    CARD8	IAReqType;		/* always X_IAQueryVersion */
    CARD16	length B16;
} xIAQueryVersionReq;
#define sz_xIAQueryVersionReq	4

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad;			/* padding */
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD16	majorVersion B16;	/* major version of IA protocol */
    CARD16	minorVersion B16;	/* minor version of IA protocol */
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
    CARD32	pad5 B32;
} xIAQueryVersionReply;
#define sz_xIAQueryVersionReply	32

typedef struct _IASetProcessInfo {
    CARD8	reqType;		/* Always IAReqCode */
    CARD8	connectionAttrType;	/* What attribute */
    CARD16	length B16;		/* Request length */
    CARD32	flags B32;		/* Request flags */
    CARD32	uid B32;		/* requestor's uid */
} xIASetProcessInfoReq;
#define sz_xIASetProcessInfoReq	12	

typedef struct _IAGetProcessInfo {
    CARD8	reqType;		/* Always IAReqCode */
    CARD8	connectionAttrType;	/* What attribute */
    CARD16	length;			/* Request length */
    CARD32	flags B32;		/* Request flags */
} xIAGetProcessInfoReq;
#define sz_xIAGetProcessInfoReq	8

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad;			/* padding */
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD32	count B32;
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
    CARD32	pad5 B32;
} xIAGetProcessInfoReply;

#define sz_xIAGetProcessInfo 32

#endif 
