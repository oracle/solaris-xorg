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
 
#pragma ident   "@(#)tsolpriv.h 1.2     06/03/07 SMI"

#ifndef	_SYS_TSOL_PRIV_H
#define	_SYS_TSOL_PRIV_H


#include <sys/priv.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum priv_ftype {
	PRIV_ALLOWED,
	PRIV_FORCED
} priv_ftype_t;

/*
 * Privilege macros.
 */

/*
 * PRIV_ASSERT(a, b) setst.privilege "b" in privilege set "a".
 */
#define	PRIV_ASSERT(a, b) (priv_addset(a, b))

/*
 * PRIV_CLEAR(a,b) clearst.privilege "b" in privilege set "a".
 */
#define	PRIV_CLEAR(a, b) (priv_delset(a, b))

/*
 * PRIV_EQUAL(set_a, set_b) is true if set_a and set_b are identical.
 */
#define	PRIV_EQUAL(a, b) (priv_isequalset(a, b))
#define	PRIV_EMPTY(a) (priv_emptyset(a))
#define	PRIV_FILL(a) (priv_fillset(a))

/*
 * PRIV_ISASSERT tests if privilege 'b' is asserted in privilege set 'a'.
 */
#define	PRIV_ISASSERT(a, b) (priv_ismember(a, b))
#define	PRIV_ISEMPTY(a) (priv_isemptyset(a))
#define	PRIV_ISFULL(a) (priv_isfullset(a))

/*
 * This macro returns 1 if all privileges asserted in privilege set "a"
 * are also asserted in privilege set "b" (i.e. if a is a subset of b)
 */
#define	PRIV_ISSUBSET(a, b) (priv_issubset(a, b))

/*
 * Takes intersection of "a" and "b" and stores in "b".
 */
#define	PRIV_INTERSECT(a, b) (priv_intersect(a, b))

/*
 * Replaces "a" with inverse of "a".
 */
#define	PRIV_INVERSE(a)  (priv_inverse(a))

/*
 * Takes union of "a" and "b" and stores in "b".
 */
#define	PRIV_UNION(a, b) (priv_union(a, b))


#define	PRIV_PROC_AUDIT_TCB	((const char *)"proc_audit")
#define	PRIV_PROC_AUDIT_APPL	((const char *)"proc_audit")
#
#define	PRIV_NET_REPLY_EQUAL	((const char *)"net_reply_equal")
#
#define	PRIV_SYS_TRANS_LABEL	((const char *)"sys_trans_label")
#define	PRIV_WIN_COLORMAP	((const char *)"win_colormap")
#define	PRIV_WIN_CONFIG		((const char *)"win_config")
#define	PRIV_WIN_DAC_READ	((const char *)"win_dac_read")
#define	PRIV_WIN_DAC_WRITE	((const char *)"win_dac_write")
#define	PRIV_WIN_DGA		((const char *)"win_dga")
#define	PRIV_WIN_DEVICES	((const char *)"win_devices")
#define	PRIV_WIN_DOWNGRADE_SL	((const char *)"win_downgrade_sl")
#define	PRIV_WIN_FONTPATH	((const char *)"win_fontpath")
#define	PRIV_WIN_MAC_READ	((const char *)"win_mac_read")
#define	PRIV_WIN_MAC_WRITE	((const char *)"win_mac_write")
#define	PRIV_WIN_SELECTION	((const char *)"win_selection")
#define	PRIV_WIN_UPGRADE_SL	((const char *)"win_upgrade_sl")

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_TSOL_PRIV_H */
