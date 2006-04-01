/* $Xorg: extnsionst.h,v 1.4 2001/02/09 02:05:15 xorgcvs Exp $ */
/***********************************************************

Copyright 1987, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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

/* $XFree86: xc/programs/Xserver/include/extnsionst.h,v 3.8 2003/04/27 21:31:04 herrb Exp $ */

#ifndef EXTENSIONSTRUCT_H
#define EXTENSIONSTRUCT_H 

#include "misc.h"
#include "screenint.h"
#include "extension.h"
#include "gc.h"

#ifdef TSOL
#include "inputstr.h"
#endif /* TSOL */

typedef struct _ExtensionEntry {
    int index;
    void (* CloseDown)(	/* called at server shutdown */
	struct _ExtensionEntry * /* extension */);
    char *name;               /* extension name */
    int base;                 /* base request number */
    int eventBase;            
    int eventLast;
    int errorBase;
    int errorLast;
    int num_aliases;
    char **aliases;
    pointer extPrivate;
    unsigned short (* MinorOpcode)(	/* called for errors */
	ClientPtr /* client */);
#ifdef XCSECURITY
    Bool secure;		/* extension visible to untrusted clients? */
#endif
} ExtensionEntry;

/* 
 * The arguments may be different for extension event swapping functions.
 * Deal with this by casting when initializing the event's EventSwapVector[]
 * entries.
 */
typedef void (*EventSwapPtr) (xEvent *, xEvent *);

extern EventSwapPtr EventSwapVector[128];

extern void NotImplemented (	/* FIXME: this may move to another file... */
	xEvent *,
	xEvent *);

typedef void (* ExtensionLookupProc)(
#ifdef EXTENSION_PROC_ARGS
    EXTENSION_PROC_ARGS
#else
    /* args no longer indeterminate */
    char *name,
    GCPtr pGC
#endif
);

typedef struct _ProcEntry {
    char *name;
    ExtensionLookupProc proc;
} ProcEntryRec, *ProcEntryPtr;

typedef struct _ScreenProcEntry {
    int num;
    ProcEntryPtr procList;
} ScreenProcEntry;

#define    SetGCVector(pGC, VectorElement, NewRoutineAddress, Atom)    \
    pGC->VectorElement = NewRoutineAddress;

#define    GetGCValue(pGC, GCElement)    (pGC->GCElement)


extern ExtensionEntry *AddExtension(
    char* /*name*/,
    int /*NumEvents*/,
    int /*NumErrors*/,
    int (* /*MainProc*/)(ClientPtr /*client*/),
    int (* /*SwappedMainProc*/)(ClientPtr /*client*/),
    void (* /*CloseDownProc*/)(ExtensionEntry * /*extension*/),
    unsigned short (* /*MinorOpcodeProc*/)(ClientPtr /*client*/)
);

extern Bool AddExtensionAlias(
    char* /*alias*/,
    ExtensionEntry * /*extension*/);

extern ExtensionEntry *CheckExtension(const char *extname);

extern ExtensionLookupProc LookupProc(
    char* /*name*/,
    GCPtr /*pGC*/);

extern Bool RegisterProc(
    char* /*name*/,
    GCPtr /*pGC*/,
    ExtensionLookupProc /*proc*/);

extern Bool RegisterScreenProc(
    char* /*name*/,
    ScreenPtr /*pScreen*/,
    ExtensionLookupProc /*proc*/);

extern void DeclareExtensionSecurity(
    char * /*extname*/,
    Bool /*secure*/);

#ifdef TSOL
typedef struct
{
   XID (*CheckAuthorization)(unsigned int, char *, unsigned int,
        char *, ClientPtr , char **);
    int (*InitWindow)(ClientPtr, WindowPtr);
    int (*ChangeWindowProperty)(ClientPtr, WindowPtr, Atom, Atom, int, int,
        unsigned long, pointer, Bool);
    int (*DeleteProperty)(ClientPtr, WindowPtr, Atom);
    char (*CheckPropertyAccess)(ClientPtr, WindowPtr, ATOM, Mask);
    void (*ProcessKeyboard)(xEvent *, KeyClassPtr);
    void (*DeleteClientFromAnySelections)(ClientPtr);
    void (*DeleteWindowFromAnySelections)(WindowPtr);
    void (*AuditStart)(ClientPtr);
    void (*AuditEnd)(ClientPtr, int);
} SecurityHook, *SecurityHookPtr;

extern SecurityHookPtr pSecHook;

#endif /* TSOL */

#endif /* EXTENSIONSTRUCT_H */

