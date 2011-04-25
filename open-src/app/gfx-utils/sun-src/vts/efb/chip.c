
/*
 * Copyright (c) 2006, 2009, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <errno.h>
#include <signal.h>		/* signal() */
#include <stdio.h>
#include <stropts.h>		/* ioctl() */
#include <unistd.h>		/* ioctl(), sleep() */
#include <sys/mman.h>

#include "gfx_common.h"		/* VTS Graphics Test common routines */
#include "graphicstest.h"
#include "gfx_vts.h"		/* VTS Graphics Test common routines */
#include "efb.h"		

void
box(struct efb_info *pEFB, int x1, int y1, int x2, int y2, unsigned int color)
{
	int		tmp;
	int		width;
	int		height;
	unsigned int	v;

	width  = x2 - x1;
	height = y2 - y1;

	if ((width <= 0) || (height <= 0)) {
#ifdef DEBUG
		printf("x1=%d x2=%d y1=%d y2=%d\n", x1, x2, y1, y2);
#endif
		return;
	}

	efb_wait_for_fifo(pEFB, 5);

	REGW(RADEON_DP_WRITE_MASK, 0xffffffff);
	REGW(RADEON_DP_CNTL,	
		(RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM));

	REGW(RADEON_DP_BRUSH_FRGD_CLR, color);
        REGW(DST_Y_X,  x1 << DST_Y_X__DST_X__SHIFT |
                       y1 << DST_Y_X__DST_Y__SHIFT) ;
        REGW(DST_WIDTH_HEIGHT,
              height << DST_WIDTH_HEIGHT__DST_HEIGHT__SHIFT |
              width << DST_WIDTH_HEIGHT__DST_WIDTH__SHIFT ) ;

}	/* box() */

void
line(struct efb_info *pEFB,
	int		x1,
	int		y1,
	int		x2,
	int		y2,
	unsigned int	color
	)
{
	efb_wait_for_fifo(pEFB, 5);

	REGW(RADEON_DP_WRITE_MASK, 0xffffffff);
	REGW(RADEON_DP_CNTL,	
		(RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM));

	REGW(RADEON_DP_BRUSH_FRGD_CLR, color);
        REGW(DST_LINE_START,  
		(x1 << DST_LINE_START__DST_START_X__SHIFT | y1 << DST_LINE_START__DST_START_Y__SHIFT));
        REGW(DST_LINE_END,  
		(x2 << DST_LINE_END__DST_END_X__SHIFT | y2 << DST_LINE_END__DST_END_Y__SHIFT));

}	/* line() */

#define NBOX 100

void
draw_cascaded_box(struct efb_info *pEFB, int width, int height)
{
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;
	unsigned int	w;
	unsigned int	h;
	unsigned int	i, j;
	unsigned int	k = 0;
	unsigned int    cinc = 0;
	unsigned int	xinc, yinc;

	cinc = 256 / NBOX;
	xinc = width / (NBOX * 2);
	yinc = height / (NBOX * 2);
	x1 = y1 = 0;

	for (i = 0; i < NBOX; i++) {

	    x2 = width - x1;
	    y2 = height - y1;

	    j = i * cinc;

	    k = (j<<24 | j<<16 | j<<8 | j);

	    box(pEFB, x1, y1, x2, y2, k);

	    x1 += xinc;
	    y1 += yinc;
	}

}	/* draw_cascaded_box() */


#define NLINE 128

void
draw_lines(struct efb_info *pEFB, int width, int height)
{
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;
	int		k;
	int		i;
	unsigned int	xinc, yinc;

	xinc = width / NLINE;
	yinc = height / NLINE;
	x1 = y1 = 0;


	k = 0;
	x1 = 0;
	y1 = 0;
	y2 = height;
	for (i = 0; i < NLINE; i++) {
	    k  = 0x00af0000 | (i << 8) | i;

	    x2 = x1;

	    line(pEFB, x1, y1, x2, y2, k);
	
	    x1 += xinc;
	}

	x1 = 0;
	x2 = width;
	y1 = 0;

	for (i = 0; i < NLINE; i++) {
	    k  = 0x00af0000 | (i << 8) | i;

	    y2 = y1;

	    line(pEFB, x1, y1, x2, y2, k);

	    y1 += yinc;
	}
}

int
efb_init_2D(struct efb_info *pEFB)
{
	unsigned int pitch_offset;
	unsigned int pitch;
	unsigned int offset;
	unsigned int bytepp;
	unsigned int gmc_bpp;
	unsigned int dp_datatype;
	unsigned int v;
	int i;

	switch (pEFB->bitsPerPixel) {
	case 8:
		gmc_bpp = GMC_DST_8BPP;
		dp_datatype = DST_8BPP | BRUSH_SOLIDCOLOR | SRC_DSTCOLOR;
		bytepp = 1;
		break;
	case 32:
		gmc_bpp = GMC_DST_32BPP;
		dp_datatype = DST_32BPP | BRUSH_SOLIDCOLOR | SRC_DSTCOLOR;
		bytepp = 4;
		break;
	}

	offset = REGR(CRTC_OFFSET) & 0x7ffffff;
	pitch = REGR(CRTC_PITCH) & 0x7ff;

	pitch = pitch * 8;	// was in groups of 8 pixels

	pitch_offset = 
		((pitch * bytepp / 64) << 22) |
		(offset / 1024);

        /*
         * Initialize GUI engine
         */
        efb_wait_for_idle(pEFB);

	efb_wait_for_fifo(pEFB, 5);


        REGW(DEFAULT_PITCH_OFFSET, pitch_offset);
        REGW(RADEON_DST_PITCH_OFFSET, pitch_offset);
        REGW(RADEON_SRC_PITCH_OFFSET, pitch_offset);

        REGW(DEFAULT_SC_BOTTOM_RIGHT,  
		(pEFB->screenHeight << 16) |
		(pEFB->screenWidth));

        v = (
	    GMC_SRC_PITCH_OFFSET_DEFAULT |
	    GMC_DST_PITCH_OFFSET_LEAVE |
	    GMC_SRC_CLIP_DEFAULT	|
	    GMC_DST_CLIP_DEFAULT	|
            GMC_BRUSH_SOLIDCOLOR        |
            gmc_bpp                     |
            GMC_SRC_DSTCOLOR            |
            RADEON_ROP3_P               |
            GMC_WRITE_MASK_LEAVE);

        REGW(RADEON_DP_GUI_MASTER_CNTL,  v);

#ifdef DEBUG
	printf("v=0x%x\n", v);
#endif
}




void
chip_test(return_packet *rp, int fd)
{
        struct efb_info  efb_info;
        struct efb_info  *pEFB;
        unsigned int red;
        unsigned char *fbaddr;
        int i;
        int bytepp;
        int fb_offset, fb_pitch, fb_height, fb_width;

        pEFB = &efb_info;
        pEFB->fd = fd;

        /*
         * map the registers & frame buffers memory
         */
        if (efb_map_mem(pEFB, rp, GRAPHICS_ERR_CHIP) == -1) {
            return;
        }

        /*
         * initialize efb info
         */
        if (efb_init_info(pEFB) == -1) {
            return;
        }

	if (efb_init_2D(pEFB) == -1) {
	    return;
	}


	/* 
	 * Clear screen 
	 */
	box(pEFB, 0, 0, pEFB->screenWidth, pEFB->screenHeight, 0);
	efb_wait_for_idle(pEFB);
	efb_flush_pixel_cache(pEFB);
	sleep(2);

	/*
	 * line test
	 */
	draw_lines(pEFB, pEFB->screenWidth, pEFB->screenHeight);
	efb_wait_for_idle(pEFB);
	efb_flush_pixel_cache(pEFB);
	sleep(1);

	/*
	 * fill test
	 */
	draw_cascaded_box(pEFB, pEFB->screenWidth, pEFB->screenHeight);
	efb_wait_for_idle(pEFB);
	efb_flush_pixel_cache(pEFB);
	sleep(1);
	efb_wait_for_idle(pEFB);
	efb_flush_pixel_cache(pEFB);

	/* 
	 * Clear screen 
	 */
	box(pEFB, 0, 0, pEFB->screenWidth, pEFB->screenHeight, 0xff);
	efb_wait_for_idle(pEFB);
	efb_flush_pixel_cache(pEFB);
	sleep(2);


done:
        /*
         * Unmap the registers & frame buffers memory
         */
        if (efb_unmap_mem(pEFB, rp, GRAPHICS_ERR_CHIP) == -1) {
            return;
        }


	if (close(fd) == -1) {
	    gfx_vts_set_message(rp, 1, GRAPHICS_ERR_CHIP, "error closing device\n");
	    return;
	}

}	/* chip_test() */


/* End of chip.c */
