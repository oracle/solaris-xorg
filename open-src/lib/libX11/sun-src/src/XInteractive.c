/*
 * Copyright (c) 1993, 2006, Oracle and/or its affiliates. All rights reserved.
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
 */


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

#if !defined(lint)
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#endif

#include <unistd.h>
#include <stdio.h>
#include <X11/Xlibint.h>
#include <X11/extensions/interactive.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/XInteractive.h>

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

#if USE_XCB

Bool
XCBSolarisIAQueryExtension(xcb_connection_t* conn)
{
    xcb_xia_query_extension_reply_t* re;
    xcb_xia_query_extension_cookie_t ce;
    xcb_generic_error_t* error;

    (void) memset(&ce, '\0', sizeof(ce));

    ce = xcb_xia_query_extension(conn);
    re = xcb_xia_query_extension_reply(conn, ce, &error);

    if (re && !error)
        return True;

    return False;
}


Bool
XCBSolarisIAQueryVersion(xcb_connection_t* conn,
        int* majorVersion,
        int* minorVersion)
{
    xcb_xia_query_version_reply_t* rv;
    xcb_xia_query_version_cookie_t cv;
    xcb_generic_error_t* error;

    (void) memset(&cv, '\0', sizeof(cv));

    cv = xcb_xia_query_version(conn);
    rv = xcb_xia_query_version_reply(conn, cv, &error);

    if (rv && !error) {
        *majorVersion = rv->major;
        *minorVersion = rv->minor;
        return True;
    }

    *majorVersion = 0;
    *minorVersion = 0;
    return False;
}


Bool
XCBSolarisIAGetProcessInfo(xcb_connection_t* conn,
        unsigned char** Pinfo,
        CARD32 flags,
        int* count)
{
    xcb_xia_get_process_info_cookie_t pc;
    xcb_generic_iterator_t itr;
    xcb_xia_get_process_info_reply_t* pr;
    xcb_generic_error_t* error;
    uint32_t* pinfo;

    if ((conn == NULL) || (count == NULL) || (Pinfo == NULL))
        return False;

    *Pinfo = NULL;
    *count = 0;
    (void) memset(&pc, '\0', sizeof(pc));

    pc = xcb_xia_get_process_info(conn, flags);
    pr = xcb_xia_get_process_info_reply(conn, pc, &error);

    if (pr && !error && (pr->count > 0)) {
	uint32_t* pid = xcb_xia_get_process_info_pinfo(pr);
	if (pid == NULL)
	    return False;

        pinfo = (uint32_t*) Xmalloc((size_t) (pr->count * sizeof(uint32_t)));
        if (pinfo == NULL)
            return False;

	*count = pr->count;
        (void) memcpy(pinfo, pid, (size_t) (*count * sizeof(uint32_t)));
        *Pinfo = (unsigned char *) pinfo;
        return True;
    }

    return False;
}


Bool
XCBSolarisIASetProcessInfo(xcb_connection_t* conn,
        unsigned char* Pinfo,
        CARD32 flags,
        CARD32 count)
{
    xcb_void_cookie_t pc;

    if (count == 0)
        return True;

    if ((conn == NULL) || (Pinfo == NULL))
        return False;

    pc = xcb_xia_set_process_info(conn, flags,
            (uint32_t) getuid(), count, (uint32_t *) Pinfo);

    return True;
}

#endif /* USE_XCB */

