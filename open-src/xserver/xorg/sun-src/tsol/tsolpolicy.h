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

#pragma ident	"@(#)tsolpolicy.h	1.8	09/01/22 SMI"

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


int xtsol_policy(xresource_t res_type, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);

#ifdef DEBUG	/* define this in tsolinfo.h if you want debug */
/*
 * For priv debugging messages.
 * XTSOL_FAIL : logs all failures that cause a err code to be returned
 * 		in protocol
 * XTSOL_ALLOW:	logs all privs that the client is lacking, but is allowed
 * XTSOL_IGNORE	logs all failures whose err code is not returned in protocol
 *
 * The higher no. always includes the lower no. log
 */
extern int xtsol_debug; 	/* defined xres_policy.c */
#endif /* DEBUG */

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
int modify_acl(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_devices(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_devices(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_client(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_client(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_client(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_atom(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_font(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_font(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_fontpath(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_gc(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_gc(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_font(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_font(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_cursor(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_ccell(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_ccell(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_ccell(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_cmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_cmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int install_cmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_window(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_window(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int create_window(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_window(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_pixmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_pixmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int create_pixmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_pixmap(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_property(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_property(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_property(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int create_srvgrab(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int destroy_srvgrab(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_confwin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_grabwin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_focuswin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_focuswin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_selection(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_propwin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_pixel(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_pixel(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_extn(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_sl(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_il(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_tpwin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_eventwin(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_stripe(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_wowner(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_uid(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int modify_polyinfo(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int read_iil(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int access_dbe(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);
int swap_dbe(xresource_t res, xmethod_t method,
    void *resource, void *subject, xpolicy_t policy_flags, void *misc);

void init_win_privsets(void);
void free_win_privsets(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _TSOL_POLICY_H */
