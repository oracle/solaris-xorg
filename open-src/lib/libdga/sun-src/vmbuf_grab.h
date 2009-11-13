/* Copyright 1993 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)vmbuf_grab.h	35.2	09/11/09 SMI"

/*
 * vmbuf_grab.h - Viewable multibuffer shared file definition
 */

#ifndef _VMBUF_GRAB_H
#define _VMBUF_GRAB_H

/* 
** Currently, the code for the viewable multibuffer shared info is commented
** out because there is nothing to put in it.  If something needs to be
** added, define this symbol and complete the code (warning: the code 
** for this is not yet fully complete).
*/
#undef VBSHINFO

#ifdef VBSHINFO

#define	VBMAGIC	0x47524143	/* "GRABBED VIEWABLE MULTIBUFFERS" */
#define VBVERS          4	/* multibuffer grabber version */

typedef	struct _vbinfo
{
    /* Shared data */
    long	magic;		/* magic number: VBMAGIC */
    long	version;	/* version: VBVERS */
    Dga_token	winToken;	/* opaque cookie for shinfo of main window */

    u_int	masterChngCnt;	/* bumped when anything changes in any buffer */
    u_int	chngCnts[DGA_MAX_GRABBABLE_BUFS];	
    				/* change counts for individual buffers */

    /* Server only data */
    u_long	s_filesuffix;	/* "cookie" for info file */
    u_char	*s_wxlink;	/* server's ptr to shinfo of main window */

    /* FUTURE: may include backing store information */

} VBINFO;

#endif /*VBSHINFO*/

#endif /* _VMBUF_GRAB_H */
