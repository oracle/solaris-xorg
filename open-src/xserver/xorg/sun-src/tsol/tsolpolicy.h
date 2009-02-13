/* Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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

#ifndef	_TSOL_POLICY_H
#define	_TSOL_POLICY_H

#pragma ident	"@(#)tsolpolicy.h	1.10	09/02/12 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

#include <assert.h>

#define	PASSED		0	/* success code 0 */
#ifndef FAILED
#define	FAILED		1	/* failed is non-zero (could be error no) */
#endif /* FAILED */

/*
 * Policy checking flags
 */

enum xpolicy_flags {
	TSOL_MAC   = 0x00000001,	/* MAC policy */
	TSOL_DAC   = 0x00000002,	/* DAC floating */
	TSOL_FLOAT = 0x00000004,	/* float ILs */
	TSOL_AUDIT = 0x00000008,	/* perform auditing */
	TSOL_PRIV  = 0x00000010,	/* privilege check */
	TSOL_ALL   = 0x0fffffff		/* do them all */
};

typedef enum xpolicy_flags xpolicy_t;
/*
 * Access Methods. Special access methods include:
 *	Not known yet.
 */

enum xaccess_methods {
	TSOL_READ = 0,
	TSOL_MODIFY,
	TSOL_CREATE,
	TSOL_DESTROY,
	TSOL_SPECIAL,
	TSOL_MAX_XMETHODS	/* Keep this as the last item */
};

typedef enum xaccess_methods xmethod_t;

/*
 * Resource Objects
 */
#define TSOL_START_XRES		1000	/* start with large no. */
enum xresource_types {
	TSOL_RES_ACL	=	TSOL_START_XRES,
	TSOL_RES_ATOM,
	TSOL_RES_BELL,
	TSOL_RES_BTNGRAB,
	TSOL_RES_CCELL,
	TSOL_RES_CLIENT,
	TSOL_RES_CMAP,
	TSOL_RES_CONFWIN,
	TSOL_RES_CURSOR,
	TSOL_RES_EVENTWIN,
	TSOL_RES_EXTN,
	TSOL_RES_FOCUSWIN,
	TSOL_RES_FONT,
	TSOL_RES_FONTLIST,
	TSOL_RES_FONTPATH,
	TSOL_RES_GC,
	TSOL_RES_GRABWIN,
	TSOL_RES_HOSTLIST,
	TSOL_RES_IL,
	TSOL_RES_KBDCTL,
	TSOL_RES_KBDGRAB,
	TSOL_RES_KEYGRAB,
	TSOL_RES_KEYMAP,
	TSOL_RES_MODMAP,
	TSOL_RES_PIXEL,
	TSOL_RES_PIXMAP,
	TSOL_RES_POLYINFO,
	TSOL_RES_PROPERTY,
	TSOL_RES_PROPWIN,
	TSOL_RES_IIL,
	TSOL_RES_PROP_SL,
	TSOL_RES_PROP_UID,
	TSOL_RES_PTRCTL,
	TSOL_RES_PTRGRAB,
	TSOL_RES_PTRLOC,
	TSOL_RES_PTRMAP,
	TSOL_RES_PTRMOTION,
	TSOL_RES_SCRSAVER,
	TSOL_RES_SELECTION,
	TSOL_RES_SELNWIN,
	TSOL_RES_SENDEVENT,
	TSOL_RES_SL,
	TSOL_RES_SRVGRAB,
	TSOL_RES_STRIPE,
	TSOL_RES_TPWIN,
	TSOL_RES_UID,
	TSOL_RES_VISUAL,
	TSOL_RES_WINATTR,
	TSOL_RES_WINDOW,
	TSOL_RES_WINLOC,
	TSOL_RES_WINMAP,
	TSOL_RES_WINSIZE,
	TSOL_RES_WINSTACK,
	TSOL_RES_WOWNER,
        TSOL_RES_DBE,
	TSOL_MAX_XRES_TYPES
};

typedef enum xresource_types xresource_t;
/*
 * NOTE: IF YOU ADD ANY NEW RESOURCE TYPES YOU MUST ADD A NEW ROW IN THE
 * XTSOL_policy_table in xpolicy_tables.c!!!
 */

/*
 * resource_ptr: Pointer to resource, or NULL to only use resource_id
 * resource_id: XID of resource, or 0 to only use resource_ptr
 * misc: pointer to request major op for most policy types,
 *	but has other additional information for some
 */
int xtsol_policy(xresource_t res_type, xmethod_t method, void *resource_ptr,
		 XID resource_id, void *subject, xpolicy_t policy_flags,
		 void *misc);

#define	XTSOL_FAIL	1	/* Replaces SecurityErrorOperation */
#define	XTSOL_ALLOW	2	/* Replaces SecurityAllowOperation */
#define	XTSOL_IGNORE	3	/* Replaces SecurityIgnoreOperation */


#ifndef	MAJOROP
#define	MAJOROP ((xReq *)client->requestBuffer)->reqType
#endif	/* MAJOROP */
#define	RES_TYPE(xid)	((xid) & (0x8000000F))


/*
 * Function prototypes
 */

void init_win_privsets(void);
void free_win_privsets(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _TSOL_POLICY_H */
