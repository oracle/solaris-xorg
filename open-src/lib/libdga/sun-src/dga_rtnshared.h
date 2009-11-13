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

#pragma ident	"@(#)dga_rtnshared.h	1.3	09/11/09 SMI"

#ifndef _DGA_RTNSHARED_H
#define _DGA_RTNSHARED_H

/*
 * dga_rtnshared.h - Sun Direct Graphics Access shared page include file.
 */

/*
 *  These structures and definitions are for the DGA
 *  shared memory retained window grabber.
 *
 *  Note: It is intended that grabbed retained windows are used in
 *        conjunction with grabbbed visible windows.  Therefore the
 *        visible window must be grabbed prior to grabbing the
 *        retained window.  The client then accesses the retained
 *        window structures indirectly through the dga_window
 *        structure defined for the associated grabbed visible
 *        window.
 */
#define RTN_MAGIC       0x52544E44      /* "RTND" = 0x52544E44 */
#define RTN_VERS        0               /* Current version number */
#define RTN_FILE        "/shrtndm"      /* Shared info file base name.  Full  */
                                        /* name is base name + port # + '.'  */
                                        /* + window id */
#define RTN_MAXPATH     237             /* 237 = Allocated size (256) - the  */
                                        /* length of the filename (18) - 1   */
                                        /* for the null character */
#define RTN_FAILED      0               /* Value returned from rtn funcs */
                                        /* upon failure */
#define RTN_PASSED      -1              /* Value returned from rtn funcs */
                                        /* upon success */
#define RTN_GRABBED     1               /* Flag that signifies that the */
                                        /* window is shared retained */
#define RTN_MAPPED      2               /* Flag that signifies that the */
                                        /* shared retained window is mapped  */
#define RTN_MAPCHG      4               /* Flag that signifies that the */
                                        /* shared info has gone from mapped  */
                                        /* to unmapped.  */
/*
 *  Shared Retained Information.  This structure contains the information
 *  necessary to allow the server and client to share access to a DGA
 *  retained window.  This information is located in the first page of
 *  the shared memory created by the server in response to a call to
 *  XDgaGrabRetainedWindow().  The server communicates the current state
 *  of the shared retained raster to the client through the fields within
 *  this structure.
 */

typedef struct shared_retained_info
{
    u_int       magic;                  /* magic number, "RTND"=0x52544E44   */
    u_char      version;                /* version, currently 0 */
    u_char      obsolete;               /* file obsolete information flag    */
    u_char      device;                 /* device type identifier from fbio.h*/
    u_char      cached;                 /* pixels currently cached on device */
    u_int       s_cacheseq;             /* server's cache sequence count */
    u_int       s_modified;             /*server has changed this data struct*/
    u_char      *s_wxlink;              /* server's link to WXINFO struct    */
    u_int       first_mmap_offset;      /* mmap offset to next file section  */
    u_int       device_offset;          /* offset to device specific section */
    short       width;                  /* raster width */
    short       height;                 /* raster height */
    u_int       linebytes;              /* bytes per scanline */
    int         s_fd;                   /* server's file descriptor */
    u_int       s_size;                 /* server size of pixel array */
    u_char      *s_pixels;              /* server shared pixel memory pointer*/
    u_char      fn[256];                /* shared file name */
    u_char      scr_name[32];           /* screen name for cached rasters    */
    int         c_fd;                   /* client's file descriptor */
    u_int       c_size;                 /* client size of pixel array */
    u_char      *c_pixels;              /* client shared pixel memory pointer*/
    u_int       c_modified;             /* client noticed server mods */
    u_int       c_cacheseq;             /* client noticed server cache mods  */
    u_char      bitsperpixel;           /* bits per pixel */
    u_char      pad1[155];              /* unused pad area up to 512 bytes   */
    u_char      dev_info[32];            /* device specific information */
} SHARED_RETAINED_INFO;


#endif /* _DGA_RTNSHARED_H */
