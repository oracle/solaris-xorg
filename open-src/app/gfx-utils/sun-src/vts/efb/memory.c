
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
#include "efb.h"

typedef union MemType {
	uint64_t	val[8];
	uint32_t	word[16];
	uint16_t	halfwd[32];
	uint8_t		byte[64];
} MemType;

static const int access_mode[] = {
	1, 2, 4, 8, 0
};

static const char *const access_tname[] = {
	"\tverifying byte access mode...",
	"\tverifying short access mode...",
	"\tverifying word access mode...",
	"\tverifying long long access mode...",
};

void
Write8(void *addr, uint8_t data) {
	*(uint8_t *)addr = data;
}

void
Write16(void *addr, uint16_t data) {
	*(uint16_t *)addr = data;
}

void
Write32(void *addr, uint32_t data) {
	*(uint32_t *)addr = data;
}

void
Write64(void *addr, uint64_t data) {
	*(uint64_t *)addr = data;
}

uint8_t
read8(void *addr) {
	return (*(uint8_t *)addr);
}

uint16_t
read16(void *addr) {
	return (*(uint16_t *)addr);
}

uint32_t
read32(void *addr) {
	return (*(uint32_t *)addr);
}

uint64_t
read64(void *addr) {
	return (*(uint64_t *)addr);
}


u_int
test_data() {
	u_int ret;

	ret = (u_int)mrand48();
	return (ret);
}

MemType *data;
MemType *cdata;


void
plane_change(int num_planes_, caddr_t base_, int sh, int sw)
{
	int		num_planes = num_planes_;
	caddr_t		base = base_;
	int		i;
	int		j;

	int pix_per_write, old_pix_per_write;

	/* Get memory to store data */

	int memcount = num_planes * 8;

	if (data)  free(data);
	if (cdata) free(cdata);

	data  = (MemType *)memalign(64, memcount * sizeof (MemType));
	cdata = (MemType *)memalign(64, memcount * sizeof (MemType));

	/* Write data to memory */
	for (i = 0; i < memcount; i++) {
	    for (j = 0; j < 8; j++) {
		/* Figure out the value to write */
		data[i].val[j] = ((unsigned long long)test_data() << 32)
					| test_data();
		cdata[i].val[j] = ~data[i].val[j];
	    }
	}

}	/* plane_change() */


boolean_t
write_read(int xoff, int yoff, boolean_t complement, int access_mode,
	boolean_t pass, int fb_pitch, int bytepp, caddr_t base)
{
	MemType		*dp;
	int		pitch = fb_pitch;
	int		x;
	int		y;
	int		i;
	caddr_t		mem_addr;
	caddr_t		dp_addr;
	u_int		second_rdval;
	MemType		*rdval;
	int		subscr = 0;

	if (complement) {
	    dp = cdata;
	} else {
	    dp = data;
	}

	/* Write Data to Screen */
	for (y = yoff; y < yoff + 64; y++) {
	    for (x = xoff * bytepp, i = 0;
		x < ((xoff + 64) *bytepp);
		x += access_mode, i++) {
		mem_addr = (y*pitch*bytepp) + x + base;
		/* Check which access mode to use for write */
		switch (access_mode) {
		case 8:	/* long long (8-byte) access mode */
			Write64(mem_addr, dp[subscr].val[i]);
			break;
		case 4:	/* word (4-byte) access mode */
			Write32(mem_addr, dp[subscr].word[i]);
			break;
		case 2:	/* short (2-byte) access mode */
			Write16(mem_addr, dp[subscr].halfwd[i]);
			break;
		default: /* default to byte access */
			Write8(mem_addr, dp[subscr].byte[i]);
			break;
		}
	    }
	    subscr++;
	}

	/* Read the Data From the Screen */

	rdval = (MemType *)memalign(64, (sizeof (MemType) * bytepp));

	for (y = yoff; y < yoff + 64; y++) {
	    for (x = xoff * bytepp, i = 0;
		x < ((xoff + 64) * bytepp);
		x += access_mode, i++) {
		mem_addr = (y*pitch*bytepp) + x + base;

		switch (access_mode) {
		case 8:	/* long long (8-byte) access mode */
			rdval->val[i] = read64(mem_addr);
			break;
		case 4:	/* word (4-byte) access mode */
			rdval->word[i] = read32(mem_addr);
			break;
		case 2:	/* short (2-byte) access mode */
			rdval->halfwd[i] = read16(mem_addr);
			break;
		default: /* default to byte access */
			rdval->byte[i] = read8(mem_addr);
			break;
		}
	    }

	    /* TODO: verification */
	    if (memcmp(rdval, dp[subscr].byte, 64 * bytepp) != 0) {
		switch (access_mode) {
		case 8:	/* long long (8-byte) access mode */
			for (i = 0; i < (16 * bytepp); i++) {
			    if (rdval->word[i] != dp[subscr].word[i]) {
				free(rdval);
				return (B_FALSE);
			    }
			}
			break;
		case 4:	/* word (4-byte) access mode */
			for (i = 0; i < (16  * bytepp); i++) {
			    if (rdval->word[i] != dp[subscr].word[i]) {
				free(rdval);
				return (B_FALSE);
			    }
			}
			break;
		case 2:	/* short (2-byte) access mode */
			for (i = 0; i < (32 * bytepp); i++) {
			    if (rdval->halfwd[i] != dp[subscr].halfwd[i]) {
				    free(rdval);
				    return (B_FALSE);
			    }
			}
			break;
		default: /* default to byte access */
			for (i = 0; i < (64 * bytepp); i++) {
			    if (rdval->byte[i] != dp[subscr].byte[i]) {
				free(rdval);
				return (B_FALSE);
			    }
			}
			break;
		}
	    }
	    subscr ++;
	}

	return (B_TRUE);

}	/* write_read() */


void
check_plane(int num_planes, int access_mode, char *test_name,
	    int fb_offset, int fb_pitch, int fb_height,
	    int fb_width, int bytepp, caddr_t base)
{
	int		x;
	int		y;
	int		complement;

	/* Set up refber for this plane group */
	plane_change(num_planes,  base + fb_offset, fb_width, fb_height);

	/* Cover each 64x64 chunk of screen space */
	y = 0;
	while (y < fb_height) {
	    x = 0;
	    while (x < fb_width) {
		if (x + 63 > fb_width) x = fb_width - 64;
		if (y + 63 > fb_height) y = fb_height - 64;

		/* Do each chunk twice - once normal, once complement */
		for (complement = B_FALSE;
		    	complement <= B_TRUE;
		    	complement++) {
		    write_read(x, y,
				(boolean_t)complement,
				access_mode,
				B_TRUE,
				fb_pitch,
				bytepp,
				base) ||
			    write_read(x, y,
				(boolean_t)complement,
				access_mode,
				B_FALSE,
				fb_pitch,
				bytepp,
				base);
		}

		/* Move over one 64x64 chunk */
		x += 64;
	    }

	    /* Move down one 64x64 chunk */
	    y += 64;
	}

}	/* check_plane() */


void
memory_test(return_packet *rp, int fd)
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
        if (efb_map_mem(pEFB, rp, GRAPHICS_ERR_MEMORY_MSG) == -1) {
            return;
        }

	if (efb_init_info(pEFB) == -1) {
	    return;
	}

	for (i = 0; access_mode[i] != 0; i++) {
	    check_plane((size_t) pEFB->bitsPerPixel, access_mode[i], 
			(char *)"Memory Test", 
			0, pEFB->screenWidth, pEFB->screenHeight, pEFB->screenWidth,
			pEFB->bitsPerPixel/8, (caddr_t)pEFB->FBvaddr);
	}

        /*
         * Unmap the registers & frame buffers memory
         */
        if (efb_unmap_mem(pEFB, rp, GRAPHICS_ERR_MEMORY_MSG) == -1) {
            return;
        }
}	/* memory_test() */


/* End of memory.c */
