
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
#include <errno.h>		/* errno */
#include <stdio.h>
#include <string.h>		/* strerror() */
#include <stropts.h>		/* ioctl() */
#include <unistd.h>		/* ioctl() */
#include <sys/mman.h>
#include <stdlib.h>

#include "gfx_common.h"
#include "graphicstest.h"
#include "libvtsSUNWxfb.h"	/* Common VTS library definitions */
#include "gfx_vts.h"		/* VTS Graphics Test common routines */
#include "ast.h"

static int
cmp_value(struct ast_info *pAST, int x, int y)
{
	unsigned char *addr;
	unsigned int  val;

	addr = pAST->FBvaddr + (y-1) * pAST->screenPitch + x * pAST->bytesPerPixel;
	val = read32(addr);
	if ((val != 0x0) && (val != 0x00ffffff)) {
	    printf("value at [%d %d] is different...0x%x\n", x, y, val);
	    return -1;
	}

	return 0;
}


void
dma_test(return_packet *rp, int fd)
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
        if (ast_map_mem(pAST, rp, GRAPHICS_ERR_DMA) == -1) {
            return;
        }

	/*
	 * initialize ast info
	 */
	if (ast_init_info(pAST) == -1) {
	    return;
	}

	/*
	 * for now, disable dma test when running on 8 bits
	 */
	if (pAST->bytesPerPixel == 1) {
	    goto done;
	}


	ASTEnable2D(pAST);

	if (ASTInitCMDQ(pAST) == -1) {
	    pAST->MMIO2D = 1;
	} else {
	    ASTEnableCMDQ(pAST);
	}

	ASTSetupForMonoPatternFill(pAST, 0x77ddbbee, 0x77ddbbee, 0, 0xffffff, 0xf0);
	ASTMonoPatternFill(pAST, 0x77ddbbee, 0x77ddbbee, 0, 0, 
				pAST->screenWidth, pAST->screenHeight);
	ASTWaitEngIdle(pAST);

	sleep(1);

done:
        /*
         * Unmap the registers & frame buffers memory
         */
        if (ast_unmap_mem(pAST, rp, GRAPHICS_ERR_DMA) == -1) {
            return;
        }
}	/* dma_test() */


/* End of dma.c */
