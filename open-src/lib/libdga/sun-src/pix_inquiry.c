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

#pragma ident	"@(#)pix_inquiry.c	35.2	09/11/09 SMI"

/*
** pix_inquiry.c - state inquiry routines for pixmaps.
*/

#ifdef SERVER_DGA
#include <X11/Xlib.h>
#endif /* SERVER_DGA */
#include "dga_incls.h"
#include "pix_grab.h"

Display *
dgai_pix_display(_Dga_pixmap dgapix)
{
    return (dgapix->p_dpy);
}

char *
dgai_pix_devname(_Dga_pixmap dgapix)
{
    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE)
	return (NULL);
    return((char *)(((SHARED_PIXMAP_INFO *)(dgapix->p_infop))->scr_name));
}

int
dgai_pix_devfd(_Dga_pixmap dgapix)
{
    return(dgapix->p_infofd);
}

int
dgai_pix_depth(_Dga_pixmap dgapix)
{
    return(dgapix->depth);
}

void
dgai_pix_set_client_infop(_Dga_pixmap dgapix, void *client_info_ptr)
{
    dgapix->p_client = client_info_ptr;
}

void *
dgai_pix_get_client_infop(_Dga_pixmap dgapix)
{
    return(dgapix->p_client);
}

void *
dgai_pix_devinfo(_Dga_pixmap dgapix)
{
    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE)
	return (NULL);
    return(((char *)dgapix->p_infop) + PIX_INFOP(dgapix)->device_offset);
}

int
dgai_pix_devinfochg(_Dga_pixmap dgapix)
{
    if (dgapix->changeMask & DGA_CHANGE_DEVINFO) {
	dgapix->changeMask &= ~DGA_CHANGE_DEVINFO;
	return (1);
    } else {
	return (0);
    }
}

int
dgai_pix_sitechg(_Dga_pixmap dgapix, int *reason)
{
    if (dgapix->changeMask & DGA_CHANGE_SITE) {
	dgapix->changeMask &= ~DGA_CHANGE_SITE;
	*reason = dgapix->siteChgReason;
	return (1);
    } else {
	*reason = DGA_SITECHG_UNKNOWN;
	return (0);
    }
}

void
dgai_pix_sitesetnotify(_Dga_pixmap dgapix, 
		       DgaSiteNotifyFunc site_notify_func, void *client_data)
{
    dgapix->siteNotifyFunc = site_notify_func;
    dgapix->siteNotifyClientData = client_data;
}

void
dgai_pix_sitegetnotify(_Dga_pixmap dgapix, 
		       DgaSiteNotifyFunc *site_notify_func, void **client_data)
{
    *site_notify_func = dgapix->siteNotifyFunc;
    *client_data = dgapix->siteNotifyClientData;
}

int
dgai_pix_site(_Dga_pixmap dgapix)
{
    SHARED_PIXMAP_INFO *infop;

    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE) {
        return (DGA_SITE_NULL);
    }

    infop = PIX_INFOP(dgapix);
    return (infop->cached ? DGA_SITE_DEVICE : DGA_SITE_SYSTEM);
}

void *
dgai_pix_address(_Dga_pixmap dgapix)
{
    SHARED_PIXMAP_INFO *infop;

    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE)
	return (NULL);

    infop = PIX_INFOP(dgapix);
    return(infop->cached ? NULL : (void *)(infop + 1));
}

int
dgai_pix_linebytes(_Dga_pixmap dgapix)
{
    SHARED_PIXMAP_INFO *infop;

    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE)
	return (NULL);

    infop = PIX_INFOP(dgapix);
    return(infop->cached ? 0 : infop->linebytes);
}

int
dgai_pix_bitsperpixel(_Dga_pixmap dgapix)
{
    SHARED_PIXMAP_INFO *infop;

    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE)
	return (NULL);

    infop = PIX_INFOP(dgapix);
    return(infop->cached ? 0 : infop->bitsperpixel);
}

int
dgai_pix_clipchg(_Dga_pixmap dgapix)
{
    /* always force a clip change on a zombie to get the client
       to notice */
    if ((dgapix->changeMask & DGA_CHANGE_ZOMBIE) ||
	(dgapix->pix_flags & PIX_NOTICE_CLIPCHG)){
	dgapix->pix_flags &= ~PIX_NOTICE_CLIPCHG;
        return (1);
    } else {
        return(0);
    }

}

void
dgai_pix_bbox(_Dga_pixmap dgapix, int *xp, int *yp, int *widthp, int *heightp)
{
    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE) {
        *xp = 0;
        *yp = 0;
        *widthp = 0;
        *heightp = 0;
    } else {
        SHARED_PIXMAP_INFO *infop;
 
        infop = PIX_INFOP(dgapix);
        *xp = 0;
        *yp = 0;
        *widthp = infop->width;
        *heightp = infop->height;
    }
}

int
dgai_pix_empty(_Dga_pixmap dgapix)
{
    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE) {
        return (1);
    } else {
        SHARED_PIXMAP_INFO *infop;
 
        infop = PIX_INFOP(dgapix);
        return (infop->width <= 0 || infop->height <= 0);
    }
}

static short nonviewableShape[] = {
    0, /* ymin */
    0, /* ymax: fill in based on height */
    0, /* xmin */
    0, /* xmax: fill in based on width */
    DGA_X_EOL,
    DGA_Y_EOL,
};
 
#define YMAX_INDEX 1
#define XMAX_INDEX 3

short *
dgai_pix_clipinfo(_Dga_pixmap dgapix)
{
    if (dgapix->changeMask & DGA_CHANGE_ZOMBIE) {
        return (NULL);
    } else {
        SHARED_PIXMAP_INFO *infop;
 
        infop = PIX_INFOP(dgapix);
        if (infop->width <= 0 || infop->height <= 0) {
            return (NULL);
        } else {
            nonviewableShape[YMAX_INDEX] = infop->height - 1;
            nonviewableShape[XMAX_INDEX] = infop->width - 1;
            return (nonviewableShape);
        }
    }    
}

