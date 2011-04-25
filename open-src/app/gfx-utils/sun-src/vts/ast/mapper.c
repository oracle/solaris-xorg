
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
#include <stdio.h>
#include <sys/mman.h>

#include "gfx_common.h"		/* GFX Common definitions */
#include "graphicstest.h"
#include "libvtsSUNWast.h"	/* Common VTS library definitions */

#include "X11/Xlib.h"
#include "gfx_vts.h"		/* VTS Graphics Test common routines */
#include "ast.h"


/*
 * map_me()
 */

void
map_me(return_packet *rp, int fd)
{
	struct ast_info  ast_info;
	struct ast_info  *pAST;
	unsigned int	 value, chipType;
	

	pAST = &ast_info;
	pAST->fd = fd;

	/*
	 * map the registers & frame buffers memory
	 */
	if (ast_map_mem(pAST, rp, GRAPHICS_ERR_OPEN) == -1) {
	    return;
	}

        if (ast_init_info(pAST) == -1) {
            return;
        }

	/*
	 * 32 bits support only for now
	 */
	if (pAST->bytesPerPixel == 1) {
	    goto done;
	}
	
	/*
	 * Write some registers
	 */
	pAST->write32(pAST->MMIOvaddr + 0xF004, 0x1e6e0000);
	value = pAST->read32(pAST->MMIOvaddr + 0xF004);
	if (value != 0x1e6e0000) {
	    gfx_vts_set_message(rp, 1, GRAPHICS_ERR_OPEN, "write/read registers failed");
	    return;
	}

	pAST->write32(pAST->MMIOvaddr + 0xF000, 0x1);
	value = pAST->read32(pAST->MMIOvaddr + 0xF000);
	if (value != 0x1) {
	    gfx_vts_set_message(rp, 1, GRAPHICS_ERR_OPEN, "write/read registers failed");
	    return;
	}


	value = pAST->read32(pAST->MMIOvaddr + 0x1207c);
	chipType = value & 0x0300;
#if DEBUG
	printf("chipType = 0x%x\n", chipType);
#endif


done:
	/*
	 * Unmap the registers & frame buffers memory
	 */
	if (ast_unmap_mem(pAST, rp, GRAPHICS_ERR_OPEN) == -1) {
	    return;
	}

}	/* map_me() */


/* End of mapper.c */
