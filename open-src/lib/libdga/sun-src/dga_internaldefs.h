/* Copyright 1994 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)dga_internaldefs.h	35.3	09/11/09 SMI"

#ifndef	_DGA_INTERNALDEFS_H
#define	_DGA_INTERNALDEFS_H

#include "dga_externaldefs.h"
/*
** Defined symbols and macros used internally within DGA
*/

#define DGA_MBFLIP_VIDEO		0
#define DGA_MBFLIP_COPY			1

/* Basic changes - attribute changes */
#define DGA_CHANGE_CLIP		(1L<<0)
#define DGA_CHANGE_CURSOR	(1L<<1)
#define DGA_CHANGE_BSTORE	(1L<<2)
#define DGA_CHANGE_CACHE	(1L<<3)
#define DGA_CHANGE_ZOMBIE	(1L<<4)

/* Basic changes - "major boundary" changes */
#define DGA_CHANGE_MBUFSET	(1L<<5)
#define DGA_CHANGE_ALIAS	(1L<<6)

/* Always a derivative change */
#define DGA_CHANGE_SITE		(1L<<7)

/* Device dependent information change */
#define DGA_CHANGE_DEVINFO      (1L<<8)

/* Overlay state information change */
#define DGA_CHANGE_OVLSTATE	(1L<<9)

#define DGA_WIN_CHANGEABLE_ATTRS \
    (DGA_CHANGE_CLIP | DGA_CHANGE_CURSOR | DGA_CHANGE_BSTORE)

/* note: bstore needs to be included because prev lock subject could have
   been a win with bstore and vmbufs never have bstore */
#define DGA_VMBUF_CHANGEABLE_ATTRS \
    (DGA_CHANGE_CLIP |  DGA_CHANGE_CURSOR | DGA_CHANGE_BSTORE)

/* note: bstore needs to be included because prev lock subject could have
   been a win with bstore and nmbufs never have bstore */
#define DGA_NMBUF_CHANGEABLE_ATTRS \
    (DGA_CHANGE_CLIP |  DGA_CHANGE_CURSOR | DGA_CHANGE_BSTORE | DGA_CHANGE_CACHE)

#define DGA_LOCKSUBJ_INVALID	(-2)

#define DGA_LOCKSUBJ_VALID(dgawin, lockSubj) \
    ((lockSubj) != DGA_LOCKSUBJ_INVALID)

#define DGA_MBUFFER_MASK(bufIndex) \
    (1L<<(bufIndex))

/* lock subject is a window? */
#define DGA_LOCKSUBJ_WINDOW(dgawin, lockSubj) \
    ((lockSubj) == -1)

/* lock subject is a multibuffer? */
#define DGA_LOCKSUBJ_MBUFFER(dgawin, lockSubj) \
    (!DGA_LOCKSUBJ_WINDOW((dgawin), (lockSubj)) && \
     DGA_LOCKSUBJ_VALID((dgawin), (lockSubj)))

/* lock subject is a viewable multibuffer? */
#define DGA_LOCKSUBJ_VMBUFFER(dgawin, lockSubj) \
    (DGA_LOCKSUBJ_MBUFFER((dgawin), (lockSubj)) \
         ? DGA_MBUF_ISVIEWABLE((dgawin)->pMbs, (lockSubj)) \
         : 0) \

/* effective lock subject is a nonviewable multibuffer? */
#define DGA_LOCKSUBJ_NMBUFFER(dgawin, lockSubj) \
    (DGA_LOCKSUBJ_MBUFFER((dgawin), (lockSubj)) \
         ? !DGA_LOCKSUBJ_VMBUFFER((dgawin), (lockSubj)) \
         : 0) \

/* is lock subject viewable? (zombies are considered not viewable) */
#define DGA_LOCKSUBJ_VIEWABLE(dgawin, lockSubj) \
     ( !((dgawin)->changeMask & DGA_CHANGE_ZOMBIE) && \
       ((lockSubj) != DGA_LOCKSUBJ_INVALID) && \
       ( (DGA_LOCKSUBJ_WINDOW((dgawin), (lockSubj))) || \
         (DGA_MBUF_ISVIEWABLE((dgawin)->pMbs, (lockSubj))) \
       ) \
     )

/* is window or mbuffer aliased? */
#define DGA_MBSMEMB_ALIASED(dgawin) \
    ((dgawin)->eLockSubj != (dgawin)->rLockSubj)

#define DGA_MBUF_ISVIEWABLE(pMbs, bufIndex) \
    (!((pMbs)->pNbShinfo[(bufIndex)]))

#endif /* _DGA_INTERNALDEFS_H */
