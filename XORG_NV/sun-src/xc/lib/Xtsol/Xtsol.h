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

#pragma ident   "@(#)Xtsol.h 1.6     06/03/07 SMI"

#ifndef _XTSOL_H_
#define _XTSOL_H_

#include <sys/types.h>
#include <bsm/audit.h>
#include <X11/Xmd.h>
#include <tsol/label.h>

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

typedef enum { IsWindow, IsPixmap, IsColormap } ResourceType;

/*
 * Name of the Trusted Solaris extension
 */
#define TSOLNAME "SUN_TSOL"

/* 
 * Resource value masks
 * The following resource masks are obsolete:
 * RES_IL        2      # information label
 * RES_IIL       4      # input info  label
 */

#define RES_SL        1      /* sensitivity label */
#define RES_UID       8      /* user id     */
#define RES_OUID     16      /* owner uid */
#define RES_STRIPE   32      /* screen stripe */
#define RES_LABEL    (RES_SL)
#define RES_ALL      (RES_SL|RES_UID|RES_OUID)


typedef struct _XTsolResAttributes {
    CARD32     ouid;         /* owner uid */
    CARD32     uid;
    m_label_t  *sl;           /* sensitivity label */
} XTsolResAttributes;

typedef struct _XTsolPropAttributes {
    CARD32     uid;
    m_label_t  *sl;           /* sensitivity label */
} XTsolPropAttributes;

/*
 * Client Attributes
 */
typedef struct _XTsolClientAttributes {
    int      trustflag;      /* true, if client masked as trusted */
    uid_t    uid;            /* owner uid */
    gid_t    gid;            /* group id */
    pid_t    pid;            /* process id */
    u_long   sessionid;      /* session id */
    au_id_t  auditid;        /* audit id */
    u_long   iaddr;          /* internet addr */
} XTsolClientAttributes;

/*
 * Trusted X Server Interfaces
 * Status value 0 means failure, else success
 * Status is defined in Xlib.h for user includes.
 */

#ifndef Status
#define Status int
#endif

#ifndef _XTSOL_SERVER

extern Bool XTSOLIsWindowTrusted(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win
#endif
);

extern Status XTSOLsetPolyInstInfo(
#if NeedFunctionPrototypes
    Display *dpy,
    m_label_t *sl,
    uid_t *uidp,
    int enabled
#endif
);

extern Status XTSOLsetPropLabel(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win,
    Atom property,
    m_label_t *sl
#endif
);

extern Status XTSOLsetPropUID(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win,
    Atom property,
    uid_t *uidp
#endif
);

extern Status XTSOLsetResLabel(
#if NeedFunctionPrototypes
    Display *dpy,
    XID object,
    ResourceType resourceFlag,
    m_label_t *sl
#endif
);

extern Status XTSOLsetResUID(
#if NeedFunctionPrototypes
    Display *dpy,
    XID object,
    ResourceType resourceFlag,
    uid_t *uidp
#endif
);

extern Status XTSOLsetSSHeight(
#if NeedFunctionPrototypes
    Display *dpy,
    int screen_num,
    int newHeight
#endif
);

extern Status XTSOLgetSSHeight(
#if NeedFunctionPrototypes
    Display *dpy,
    int screen_num,
    int *newHeight
#endif
);

extern Status XTSOLsetSessionHI(
#if NeedFunctionPrototypes
    Display *dpy,
    bclear_t *sl
#endif
);

extern Status XTSOLsetSessionLO(
#if NeedFunctionPrototypes
    Display *dpy,
    m_label_t *sl
#endif
);

extern Status XTSOLsetWorkstationOwner(
#if NeedFunctionPrototypes
    Display *dpy,
    uid_t *uidp
#endif
);

extern Status XTSOLgetClientAttributes(
#if NeedFunctionPrototypes
    Display *dpy,
    XID xid,
    XTsolClientAttributes *clientattr
#endif
);

extern Status XTSOLgetClientLabel(
#if NeedFunctionPrototypes
    Display *dpy,
    XID xid,
    m_label_t *sl
#endif
);

extern Status XTSOLgetPropAttributes(
#if NeedFunctionPrototypes
    Display *dpy,
    Window window,
    Atom property,
    XTsolPropAttributes *propattrp
#endif
);

extern Status XTSOLgetPropLabel(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win,
    Atom property,
    m_label_t *sl
#endif
);

extern Status XTSOLgetPropUID(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win,
    Atom property,
    uid_t *uidp
#endif
);

extern Status XTSOLgetResAttributes(
#if NeedFunctionPrototypes
    Display *dpy,
    XID object,
    ResourceType resourceFlag,
    XTsolResAttributes *resattrp
#endif
);

extern Status XTSOLgetResLabel(
#if NeedFunctionPrototypes
    Display *dpy,
    XID object,
    ResourceType resourceFlag,
    m_label_t *sl
#endif
);

extern Status XTSOLgetResUID(
#if NeedFunctionPrototypes
    Display *dpy,
    XID object,
    ResourceType resourceFlag,
    uid_t *uidp
#endif
);

extern Status XTSOLgetWorkstationOwner(
#if NeedFunctionPrototypes
    Display *dpy,
    uid_t *uidp
#endif
);

extern Status XTSOLMakeTPWindow(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win
#endif
);

extern Status XTSOLMakeTrustedWindow(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win
#endif
);

extern Status XTSOLMakeUntrustedWindow(
#if NeedFunctionPrototypes
    Display *dpy,
    Window win
#endif
);

#endif /* _XTSOL_SERVER */

_XFUNCPROTOEND

#endif /* _XTSOL_H_ */
