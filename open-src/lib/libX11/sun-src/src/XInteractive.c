/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)XInteractive.c	35.9	06/10/05 SMI"

/*

	Client side XIA extension code.

	One can XSolarisIASetProceesInfo or XIASolarisGetProceesInfo.
	Currently only passing process ids is supported.

	Of course QueryVersion and QueryExtension are supported.

	No support for error messages is involved. Yet. If this
	interface becomes public this will need to be rewhacked
 */

/* THIS IS NOT AN X CONSORTIUM STANDARD */

#define NEED_EVENTS
#define NEED_REPLIES
#include <unistd.h>
#include <stdio.h>
#include <X11/Xlibint.h>
#include <X11/extensions/interactive.h>
#include <X11/extensions/Xext.h>

#ifdef IA_USE_EXTUTIL
#include <X11/extensions/extutil.h>

static XExtensionInfo	_ia_info_data;
static XExtensionInfo	*ia_info = &_ia_info_data;

static /* const */ char	*ia_extension_name = IANAME;

#define IACheckExtension(dpy,i,val) \
  XextCheckExtension (dpy, i, ia_extension_name, val)
#else
typedef struct _IAExtDisplayInfo {
  Display *display;
  XExtCodes *codes;
  struct _IAExtDisplayInfo *next;
} IAExtDisplayInfo;

static IAExtDisplayInfo *iaExtDisplayList = NULL;
static const char *ia_extension_name = IANAME;

#define XExtDisplayInfo IAExtDisplayInfo

#define XextHasExtension(i)  (((i) != NULL) && ((i)->codes != NULL))

#define IACheckExtension(dpy,i,val) \
    if (!XextHasExtension(i)) { \
        /* XMissingExtension (dpy, ia_extension_name); */ \
	return val; \
    }

#endif /* IA_USE_EXTUTIL */

/*****************************************************************************
 *                                                                           *
 *			   private utility routines                          *
 *                                                                           *
 *****************************************************************************/

#ifdef IA_USE_EXTUTIL
static int close_display();
static char *error_string();
static /* const */ XExtensionHooks ia_extension_hooks = {
    NULL,				/* create_gc */
    NULL,				/* copy_gc */
    NULL,				/* flush_gc */
    NULL,				/* free_gc */
    NULL,				/* create_font */
    NULL,				/* free_font */
    close_display,			/* close_display */
    NULL,				/* wire_to_event */
    NULL,				/* event_to_wire */
    NULL,				/* error */
    error_string,			/* error_string */
};
#endif

static const char *ia_error_list[] = {
    "BadPid",			/* Bad process id */
};

#ifdef IA_USE_EXTUTIL
static XEXT_GENERATE_FIND_DISPLAY (find_display, ia_info, ia_extension_name, 
				   &ia_extension_hooks, IANumberEvents, NULL)

static XEXT_GENERATE_CLOSE_DISPLAY (close_display, ia_info)

static XEXT_GENERATE_ERROR_STRING (error_string, ia_extension_name,
				   IANumberErrors, ia_error_list)
#else
static int ia_close_display(Display *dpy, XExtCodes *codes)
{
    IAExtDisplayInfo *di, *prev, *next;

    for (di = iaExtDisplayList, prev = NULL; di != NULL; di = next) {
	next = di->next;
	if (di->display == dpy) {
	    if (prev != NULL) {
		prev->next = di->next;
	    } else {
		iaExtDisplayList = di->next;
	    }
	    Xfree(di);
	} else {
	    prev = di;
	}
    }
    return Success;
}

static char *ia_error_string (Display *dpy, int err, XExtCodes *codes,
			      char *buf, int nbytes) 
{
    err -= codes->first_error;
    if (err >= 0 && err < IANumberErrors) {
        char tmp[256];
        snprintf (tmp, sizeof(tmp), "%s.%d", ia_extension_name, err);
        XGetErrorDatabaseText (dpy, "XProtoError", tmp, ia_error_list[err], 
			       buf, nbytes);
        return buf;
    }
    return NULL;
}

static IAExtDisplayInfo *ia_find_display(Display *dpy) 
{
    IAExtDisplayInfo *di;

    for (di = iaExtDisplayList ; di != NULL; di = di->next) {
	if (di->display == dpy) {
	    return di;
	}
    }
    /* Did not find on list, add new entry */
    di = (IAExtDisplayInfo *) Xmalloc(sizeof(IAExtDisplayInfo));
    if (di == NULL) { return NULL; }
    di->display = dpy;
    di->codes = XInitExtension(dpy, ia_extension_name);
    di->next = iaExtDisplayList;
    iaExtDisplayList = di;
    XESetCloseDisplay(dpy, di->codes->extension, ia_close_display);
    XESetErrorString(dpy, di->codes->extension, ia_error_string);
    return di;
}

#define find_display ia_find_display
#endif /* IA_USE_EXTUTIL */

/*****************************************************************************
 *                                                                           *
 *		    public Interactive Extension routines                  *
 *                                                                           *
 *****************************************************************************/

Bool
XSolarisIAQueryExtension(
    Display		*dpy
)
{
    XExtDisplayInfo	*info = find_display(dpy);

    if (XextHasExtension(info)) {
	return True;
     } else {
	return False;
    }
}


Bool
XSolarisIAQueryVersion(
    Display			*dpy,
    int				*majorVersion,
    int				*minorVersion
)
{
    XExtDisplayInfo		*info = find_display(dpy);
    xIAQueryVersionReply	 rep;
    xIAQueryVersionReq		*req;

    IACheckExtension(dpy, info, False);

    LockDisplay(dpy);
    GetReq(IAQueryVersion, req);
    req->reqType   = info->codes->major_opcode;
    req->IAReqType = X_IAQueryVersion;
    if (!_XReply(dpy, (xReply *)(&rep), 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return False;
    }
    *majorVersion = rep.majorVersion;
    *minorVersion = rep.minorVersion;
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}


Bool
XSolarisIAGetProcessInfo(
    Display				*dpy,
    unsigned char		       **Pinfo,
    CARD32				 flags,
    int					*count
)
{
    XExtDisplayInfo			*info = find_display(dpy);
    xIAGetProcessInfoReq		*req;
    xIAGetProcessInfoReply		 rep;
    long				 length = 0;
 
    IACheckExtension(dpy, info, False);

    *Pinfo = NULL;

    LockDisplay(dpy);
    GetReq(IAGetProcessInfo, req);
    req->reqType            = info->codes->major_opcode;
    req->connectionAttrType = X_IAGetProcessInfo;
    req->flags              = flags;
    if (!_XReply(dpy, (xReply *)(&rep), 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return False;
    }
    *count = rep.count;
    *Pinfo = (unsigned char *)Xmalloc((rep.count) * sizeof(ConnectionPidRec));
    if (*Pinfo == NULL) {
            UnlockDisplay(dpy);
            SyncHandle();
            return False;   /* not Success */
    }
    length = rep.count << 2;
    _XRead32(dpy, (long *)(*Pinfo), length);
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}


Bool
XSolarisIASetProcessInfo(
    Display				*dpy,
    unsigned char			*Pinfo,
    CARD32				 flags,
    CARD32				 count
)
{
    XExtDisplayInfo			*info = find_display(dpy);
    xIASetProcessInfoReq		*req;
    unsigned long			length;

    IACheckExtension(dpy, info, False);

    LockDisplay(dpy);
    GetReq(IASetProcessInfo, req);
    req->reqType            = info->codes->major_opcode;
    req->connectionAttrType = X_IASetProcessInfo;
    req->flags              = flags;
    req->length            += count;
    req->uid                = (CARD32)getuid();
    if (flags & INTERACTIVE_INFO) {
        length=count << 2;
	Data32(dpy, (long *)Pinfo, length);
    }
    if (flags & INTERACTIVE_SETTING) {
        length = count << 2;
	Data32(dpy, (long *)Pinfo, length);
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}

