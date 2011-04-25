
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
#include "ast.h"		

void
box(struct ast_info *pAST, int x1, int y1, int x2, int y2, unsigned int color)
{
	int		tmp;
	int		width;
	int		height;

	if (x2 < x1) {
	    tmp = x2;
	    x2  = x1;
	    x1  = tmp;
	}
	if (y2 < y1) {
	    tmp = y2;
	    y2  = y1;
	    y1  = tmp;
	}

	width  = x2 - x1;
	height = y2 - y1;

	ASTSetupForSolidFill(pAST, color, 0xf0);
	ASTSolidFillRect(pAST, x1, y1, width, height);

}	/* box() */

void
line(struct ast_info *pAST,
	int		x1,
	int		y1,
	int		x2,
	int		y2,
	unsigned int	color
	)
{
	ASTSetupForSolidLine(pAST, color, 0xf0);
	ASTSolidLine(pAST, x1, y1, x2, y2);

}	/* line() */


void
draw_cascaded_box(struct ast_info *pAST, int width, int height)
{
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;
	unsigned int	w;
	unsigned int	h;
	int		i;
	unsigned int	k = 0;

	for (i = 0; i < 256; i++) {

	    x1 = (unsigned int)((width * i) / 512);
	    x2 = width - x1;
	    w  = x2 - x1;

	    y1 = (unsigned int)((height * i) / 512);
	    y2 = height - y1;

	    k = (i<<24 | i<<16 | i<<8 | i);

	    box(pAST, x1, y1, x2, y2, k);
	}

}	/* draw_cascaded_box() */


void
draw_lines(struct ast_info *pAST, int width, int height)
{
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;
	int		k;
	int		i;
	int		nlines = 128;

	k = 0;
	for (i = 0; i < nlines; i++) {
	    k  = 0x00af0000 | (i << 8) | i;

	    x1 = (unsigned int)((width * i) / nlines);
	    x2 = x1;
	    y1 = 0;
	    y2 = height;

	    line(pAST, x1, y1, x2, y2, k);
	}

	for (i = 0; i < nlines; i++) {
	    k  = 0x00af0000 | (i << 8) | i;

	    x1 = 0;
	    x2 = width;
	    y1 = (unsigned int)((height * i) / nlines);
	    y2 = y1;

	    line(pAST, x1, y1, x2, y2, k);
	}
}

void
chip_test(return_packet *rp, int fd)
{
        struct ast_info  ast_info;
        struct ast_info  *pAST;
        unsigned int red;
        unsigned char *fbaddr;
        int i;
        int bytepp;
        int fb_offset, fb_pitch, fb_height, fb_width;

        pAST = &ast_info;
        pAST->fd = fd;

        /*
         * map the registers & frame buffers memory
         */
        if (ast_map_mem(pAST, rp, GRAPHICS_ERR_CHIP) == -1) {
            return;
        }

        /*
         * initialize ast info
         */
        if (ast_init_info(pAST) == -1) {
            return;
        }

	/*
	 * only support 32 bits depth for now
	 */
	if (pAST->bytesPerPixel == 1) {
	    goto done;
	}

	/*
	 * enable 2D, initialize command queue
	 */
        ASTEnable2D(pAST);

        if (ASTInitCMDQ(pAST) == -1) {
	    pAST->MMIO2D = 1;
	} else {;
            ASTEnableCMDQ(pAST);
	}

	ASTSaveState(pAST);

	/*
	 * set clipping rectangle
	 */
	ASTSetClippingRectangle(pAST, 0, 0, pAST->screenWidth, pAST->screenHeight);

	/* 
	 * Clear screen 
	 */
	box(pAST, 0, 0, pAST->screenWidth, pAST->screenHeight, 0);
	ASTWaitEngIdle(pAST);

	/*
	 * line test
	 */
	draw_lines(pAST, pAST->screenWidth, pAST->screenHeight);
	ASTWaitEngIdle(pAST);
	sleep(2);

	/*
	 * fill test
	 */
	draw_cascaded_box(pAST, pAST->screenWidth, pAST->screenHeight);
	ASTWaitEngIdle(pAST);
	sleep(2);

	/* 
	 * Clear screen 
	 */
	box(pAST, 0, 0, pAST->screenWidth, pAST->screenHeight, 0xff);
	ASTWaitEngIdle(pAST);
	sleep(2);

	ASTResetState(pAST);

done:
        /*
         * Unmap the registers & frame buffers memory
         */
        if (ast_unmap_mem(pAST, rp, GRAPHICS_ERR_CHIP) == -1) {
            return;
        }


	if (close(fd) == -1) {
	    gfx_vts_set_message(rp, 1, GRAPHICS_ERR_CHIP, "error closing device\n");
	    return;
	}

}	/* chip_test() */


/* End of chip.c */
