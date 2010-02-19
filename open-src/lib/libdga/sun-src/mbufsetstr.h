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


#ifndef	_DGA_MBUFSETSTR_H
#define	_DGA_MBUFSETSTR_H

/*
** Client structure for multibuffer set.
*/

/*
** A "multibuffer set" (mbufset) stores client-side info about the multibuffers
** attached to a multibuffered window.
*/

typedef struct dga_mbufset {
    unsigned int	refcnt;
    short		numBufs;
    _Dga_pixmap		pNbPixmaps[DGA_MAX_GRABBABLE_BUFS];	

    /* shared info for nonviewable mbufs.  Indices that
       correspond to viewable mbufs are always NULL */
    SHARED_PIXMAP_INFO	*pNbShinfo[DGA_MAX_GRABBABLE_BUFS];	

    u_int		mbufseq[DGA_MAX_GRABBABLE_BUFS];
    u_int		clipseq[DGA_MAX_GRABBABLE_BUFS];
    u_int		curseq[DGA_MAX_GRABBABLE_BUFS];

    /* last recorded cache count for nonviewable mbufs.  Indices that
       correspond to viewable mbufs are always 0 */
    u_int		cacheSeqs[DGA_MAX_GRABBABLE_BUFS];	

    /* last recorded cache count for nonviewable mbufs.  Indices that
       correspond to viewable mbufs are always 0 (viewable mbufs 
       don't have devce info) */
    u_int		devInfoSeqs[DGA_MAX_GRABBABLE_BUFS];	

    /* Has the buffer been locked previously? */
    char		prevLocked[DGA_MAX_GRABBABLE_BUFS];

} DgaMbufSet;


#endif /* _DGA_MBUFSETSTR_H */

