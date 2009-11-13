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

#pragma ident	"@(#)pix_grab.h	35.2	09/11/09 SMI"

#ifndef _PIX_GRAB_H
#define _PIX_GRAB_H

#ifndef PMODIF          	
#define PMODIF  1       	/* server has set a new cache */
#define PIX_FILE "/shpxm"  	/* Shared info file base name*/
                    		/* name is base name + port # + '.'  */
                    		/* + pixmap id               */
#define PIX_FAILED  0           /* Fail Value returned from pix funcs     */
#define PIX_PASSED  -1          /* Pass Value returned from pix funcs     */

/* client structure's pix_flag values */
#define PIX_NOTICE_CLIPCHG	1

/*
 *  Shared Pixmap Information.  This structure contains the information
 *  necessary to allow the server and client to share access to a DGA
 *  pixmap .  This information is located in the first page of
 *  the shared memory created by the server in response to a call to
 *  XDgaGrabPixmap().  The server communicates the current state
 *  of the shared pixmap to the client through the fields within
 *  this structure.
 */
 
typedef struct shpx_client_entry {
    u_int     cid;         /* client id */
    SHPX_DIRECTORY  *shpx_directory; /* server's per client shpx dir */
    u_int     fd;      	   /* server's per client file descrptr*/
    u_int     size;        /* client file size */
    u_int     npix;        /* number of pixmaps using this entry */
} SHPX_CLIENT_ENTRY;


/*
 *  Internal DGA Pixmap Macros
 */
 
#define PIX_INFOP(clientp) ((SHARED_PIXMAP_INFO *) ((clientp)->p_infop))

/* definitions of lock marcos for pixmaps - move this to dga.h later */


/* DGA_LOCK_SRC_AND_DST will go here after they are debugged */ 
#endif  /* PMODIF */
#endif /* _PIX_GRAB_H */
