/*
 * Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#ifndef	_EFB_VIS_H
#define	_EFB_VIS_H

#ifdef __cplusplus

extern "C" {
#endif

#include <sys/types.h>
#include <sys/visual_io.h>
#include "drmP.h"
#include "drm.h"
#include "radeon_drm.h"
#include "radeon_drv.h"
#include "efb_vis.h"

#if VIS_CONS_REV > 2
#define	EFB_MAX_LINEBYTES	2048	/* Number of bytes for each pixel row */
#define	EFB_MAX_PIXBYTES	4	/* Number of bytes per pixel */

/*
 * Define a default character width and height.
 */
#define	DEFCHAR_WIDTH	16 	/* Default char wid. (optimize blit bufsize) */
#define	DEFCHAR_HEIGHT	22	/* Default char ht.  (ptimize blit bufsize) */
#define	DEFCHAR_SIZE	(DEFCHAR_WIDTH * DEFCHAR_HEIGHT)

#define	SHOW_CURSOR	1	/* display cursor and save data beneath */
#define	HIDE_CURSOR	0	/* hide cursor, restoring previous data */

#define	BLUE		24	/* 32-bit pixel blue byte shift */
#define	GREEN		16	/* 32-bit pixel green byte shift */
#define	RED		8	/* 32-bit pixel red byte shift */

#define	CSRMAX		(32 * 32)  /* Cursor size max  */

/*
 * This structure defines terminal emulator state that needs to
 * be saved and/or passed between functions.
 */
struct efb_consinfo {
	uint32_t	bufsize;		/* size of blitbuf (can grow) */
	uchar_t		*bufp;			/* pointer to blit buffer */
	void *		*csrp;			/* ptr to under-csrc save buf */
	uint16_t	kcmap[2][3][256];	/* kernel's cmap */
	uint8_t		kcmap_flags[2][256];	/* active cmap entry flags */
	uint8_t		kcmap_max;		/* highest used cmap bit set */
	uint32_t	pitch;			/* Linesize incl offscr to rt */
	uint32_t	bgcolor;		/* Bkgd color for cur depth */
	uint32_t	stream;			/* Console's stream */
	uint32_t	offset;			/* offset to the framebuffer */
	uint32_t	rshift;			/* red shift */
	uint32_t	gshift;			/* green shift */
	uint32_t	bshift;			/* blue shift */
	void *		dfb;			/* Dumb framebuffer address */
	void *		dfb_handle;		/* Opaque handle for map call */
	vis_modechg_cb_t te_modechg_cb;		/* callback into TEM. */
	struct vis_modechg_arg *te_ctx;		/* opaque argument    */
	struct vis_polledio *polledio;		/* conveys polled entry pts */
};

/*
 * This structure contains the data that will be written to the hardware
 * Rendering Engine to perform an on-screen rectangle copy.   This feature
 * is used for vertical scrolling as well as for quick line inserts and deletes,
 * as per escape sequences facilities such as vi use.
 */
struct efb_vis_copy {
	uint32_t	source_x;	/* copy-from X coordinate */
	uint32_t	source_y;	/* copy-from Y coordinate */
	uint32_t	dest_x;		/* copy-to X coordinate */
	uint32_t	dest_y;		/* copy-to Y coordinate */
	uint32_t	width;		/* width of rectangle to copy */
	uint32_t	height;		/* height of rectangle to copy */
};

/*
 * This structure holds parameters describing incoming blit to display on the
 * dumb framebuffer
 */
struct efb_vis_draw_data {
	uint32_t	image_row;	/* row at which to display blit */
	uint32_t	image_col;	/* col at which to display blit */
	uint16_t	image_width;	/* width of blit */
	uint16_t	image_height;	/* height of blit */
	uint8_t		*image;		/* location of raw blit data */
};

/*
 * This structure holds parameters describing incoming blit to display on the
 * dumb framebuffer
 */
struct efb_vis_fill_data {
	uint32_t	image_row;	/* row at which to display blit */
	uint32_t	image_col;	/* col at which to display blit */
	uint16_t	image_width;	/* width of blit */
	uint16_t	image_height;	/* height of blit */
	uint32_t	color;		/* fill color */
};

#endif /* VIS_CONS_REV > 2 */

#ifdef __cplusplus
}
#endif

#endif /* _EFB_VIS_H */
