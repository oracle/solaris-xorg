
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
#include "libvtsSUNWefb.h"	/* Common VTS library definitions */

#include "X11/Xlib.h"
#include "gfx_vts.h"		/* VTS Graphics Test common routines */
#include "efb.h"


int
efb_get_pci_info(int fd, struct pci_info *pciInfo)
{
	struct gfx_pci_cfg pciCfg;
	int i;
	unsigned int bar;

	if (ioctl(fd, GFX_IOCTL_GET_PCI_CONFIG, &pciCfg) == -1) {
		return -1;
	}

        pciInfo->deviceID = pciCfg.DeviceID;

	for (i = 0; i < 6; i++) {
	    bar = pciCfg.bar[i];
	    if (bar != 0) {
		if (bar & PCI_MAP_IO) {
		    pciInfo->ioBase[i] = PCIGETIO(bar);
		    pciInfo->type[i] = bar & PCI_MAP_IO_ATTR_MASK;
		} else {
		    pciInfo->type[i] = bar & PCI_MAP_MEMORY_ATTR_MASK;
		    pciInfo->memBase[i] = PCIGETMEMORY(bar);
		    if (PCI_MAP_IS64BITMEM(bar)) {
			if (i == 5) {
			    pciInfo->memBase[i] = 0;
			} else {
			    int bar_hi = pciCfg.bar[i+1];
			    pciInfo->memBase[i] |= (bar_hi << 32);
			    ++i;
			}
		    }
		    pciInfo->size[i] = EFB_REG_SIZE_LOG2;
		}
	    }
	}

	return 0;
}

int
efb_get_mem_info(struct pci_info *pci_info, struct efb_info *pEFB) 
{
	unsigned char reg;

	pEFB->FBPhysAddr = pci_info->memBase[0] & 0xfff00000;
	pEFB->FBMapSize = 0;

	pEFB->MMIOPhysAddr = pci_info->memBase[2] & 0xffff0000;
	pEFB->MMIOMapSize =  1 << pci_info->size[2];

	pEFB->RelocateIO = pci_info->ioBase[1];

	pEFB->ChipSet = pci_info->deviceID;

#ifdef DEBUG
	printf("FBPhysAddr=0x%x FBMapSize=0x%x\n", pEFB->FBPhysAddr, pEFB->FBMapSize);
	printf("MMIOPhysAddr=0x%x MMIOMapSize=0x%x\n", pEFB->MMIOPhysAddr, pEFB->MMIOMapSize);
	printf("RelocateIO=0x%x\n", pEFB->RelocateIO);
#endif

	return 0;
}


int
efb_init_info(struct efb_info *pEFB)
{
	unsigned int v, v2;
	unsigned int status = 0;

        
#if 0
	/* 
	 * first check if the hardware is already initialized.
	 * If not, abort
	 */
	ioctl(pEFB->fd, EFB_GET_STATUS_FLAGS, &status);
	if (!(status & EFB_STATUS_HW_INITIALIZED))
		return -1;
#endif


        v = REGR(CRTC_GEN_CNTL);
	v2 = (v & CRTC_GEN_CNTL__CRTC_PIX_WIDTH_MASK) >> CRTC_GEN_CNTL__CRTC_PIX_WIDTH__SHIFT;
	if (v2 <= 2)
        	pEFB->bitsPerPixel = 8;
	else if (v2 <= 4)
		pEFB->bitsPerPixel = 16;
	else 
		pEFB->bitsPerPixel = 32;


        v = REGR(CRTC_H_TOTAL_DISP);
	v2 = (v & CRTC_H_TOTAL_DISP__CRTC_H_DISP_MASK) >> CRTC_H_TOTAL_DISP__CRTC_H_DISP__SHIFT;
	pEFB->screenWidth = (v2 + 1) * 8;

        v = REGR(CRTC_V_TOTAL_DISP);
	v2 = (v & CRTC_V_TOTAL_DISP__CRTC_V_DISP_MASK) >> CRTC_V_TOTAL_DISP__CRTC_V_DISP__SHIFT;
	pEFB->screenHeight = (v2 + 1);
	pEFB->screenPitch = pEFB->screenWidth * pEFB->bitsPerPixel / 8;


	pEFB->fbLocation = REGR(RADEON_MC_FB_LOCATION);

#ifdef DEBUG
	printf("bpp=%d width=%d height=%d pitch=%d fbLoc=0x%x\n", 
		pEFB->bitsPerPixel, pEFB->screenWidth, pEFB->screenHeight, pEFB->screenPitch, 
		pEFB->fbLocation);
#endif
	return 0;
}

int
efb_map_mem(struct efb_info *pEFB, return_packet *rp, int test)
{
        struct pci_info  pci_info;
        int              pageSize, size;
	int		 fd = pEFB->fd;

        if (efb_get_pci_info(fd, &pci_info) == -1) {
            TraceMessage(VTS_DEBUG, __func__, "get pci info failed\n");
            gfx_vts_set_message(rp, 1, test, "get pci info failed");
            return -1;
        }


        if (efb_get_mem_info(&pci_info, pEFB) == -1) {
            TraceMessage(VTS_DEBUG, __func__, "get mem info failed\n");
            gfx_vts_set_message(rp, 1, test, "get mem info failed");
            return -1;
        }

        /*
         * Map MMIO
         */
        pageSize = getpagesize();
        size = pEFB->MMIOMapSize + (pageSize - 1) & (~(pageSize - 1));

        pEFB->MMIOvaddr = (unsigned char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                                        fd, pEFB->MMIOPhysAddr);

        if (pEFB->MMIOvaddr == MAP_FAILED) {
            TraceMessage(VTS_DEBUG, __func__, "map MMIO failed\n");
            gfx_vts_set_message(rp, 1, test, "map MMIO failed");
            return -1;
        }



	switch (pEFB->ChipSet) {
	case 20825:
	case 20830:
	case 23396:
		pEFB->FBMapSize = REGR(RADEON_CONFIG_MEMSIZE);
		break;
	default:
		pEFB->FBMapSize = REGR(R600_CONFIG_MEMSIZE);
		break;
	}
	

        /*
         * Map framebuffer
         */
        pageSize = getpagesize();
        size = pEFB->FBMapSize + (pageSize - 1) & (~(pageSize - 1));

        pEFB->FBvaddr = (unsigned char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                                        fd, pEFB->FBPhysAddr);

        if (pEFB->FBvaddr == MAP_FAILED) {
            TraceMessage(VTS_DEBUG, __func__, "map framebuffer failed\n");
            gfx_vts_set_message(rp, 1, test, "map framebuffer failed");
            return -1;
        }


	return 0;
}

int
efb_unmap_mem(struct efb_info *pEFB, return_packet *rp, int test)
{
        /*
         * Unmap Frame Buffer
         */

        if (munmap((void *)pEFB->FBvaddr, pEFB->FBMapSize) == -1) {
            TraceMessage(VTS_DEBUG, __func__, "unmap framebuffer failed\n");
            gfx_vts_set_message(rp, 1, test, "unmap framebuffer failed");
            return -1;
        }

        if (munmap((void *)pEFB->MMIOvaddr, pEFB->MMIOMapSize) == -1) {
            TraceMessage(VTS_DEBUG, __func__, "unmap MMIO failed\n");
            gfx_vts_set_message(rp, 1, test, "unmap MMIO failed");
            return -1;
        }

	return 0;
}


#define MASK    RBBM_STATUS__CMDFIFO_AVAIL_MASK

uint32_t
getBPPValue (int bpp)
{
	switch (bpp) {
            case 8:     return (DST_8BPP);
            case 15:    return (DST_15BPP);
            case 16:    return (DST_16BPP);
            case 32:    return (DST_32BPP);
            default:    return (0);
    	}
}

void 
efb_flush_pixel_cache(struct efb_info *pEFB)
{
	int i ;

	// initiate flush
	REGW(RADEON_RB2D_DSTCACHE_CTLSTAT, REGR(RADEON_RB2D_DSTCACHE_CTLSTAT) | 0xf);

	// check for completion but limit looping to 16384 reads
	i = 16384;
	while((REGR(RADEON_RB2D_DSTCACHE_CTLSTAT) & RADEON_RB2D_DC_BUSY) == RADEON_RB2D_DC_BUSY &&
                --i >= 0 ) ;
}

void 
efb_reset_engine (struct efb_info *pEFB)
{
	uint32_t save_genresetcntl, save_clockcntlindex, save_mclkcntl;
	long term_count;

	efb_flush_pixel_cache(pEFB) ;

	save_clockcntlindex = REGR(RADEON_CLOCK_CNTL_INDEX);

	// save GEN_RESET_CNTL register
	save_genresetcntl = REGR(RADEON_DISP_MISC_CNTL);

	// reset by setting bit, add read delay, then clear bit, 
	// add read delay
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl |
      	     RADEON_DISP_MISC_CNTL__SOFT_RESET_GRPH_PP);
	REGR(RADEON_DISP_MISC_CNTL);
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl &
             ~(RADEON_DISP_MISC_CNTL__SOFT_RESET_GRPH_PP));
	REGR(RADEON_DISP_MISC_CNTL);

	// restore the two registers we changed
	REGW(RADEON_CLOCK_CNTL_INDEX, save_clockcntlindex);
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl);

	term_count++;     // for monitoring engine hangs
}

void 
efb_wait_for_fifo(struct efb_info *pEFB, int c)
{
	uint32_t i;
	long limit;

	/* First a short loop, just in case fifo clears out quickly */
	for(limit=100; (REGR(RBBM_STATUS) & MASK) < c && --limit > 0; ) ;

	if((REGR(RBBM_STATUS) & MASK) < c ) {
    		hrtime_t timeout = gethrtime() + (hrtime_t)3000000000;
            	while((REGR(RBBM_STATUS) & MASK) < c && 
					gethrtime() < timeout )
        	    yield();

       	    	if((REGR(RBBM_STATUS) & MASK) < c )
      	    	    efb_reset_engine(pEFB) ;
	}
}

void 
efb_wait_for_idle(struct efb_info *pEFB)
{
	uint32_t i;
	long limit;

	efb_wait_for_fifo(pEFB, 64) ;

	for(limit=10000; (REGR(RBBM_STATUS) & GUI_ACTIVE) && --limit > 0; ) ;

	if( REGR(RBBM_STATUS) & GUI_ACTIVE ) {
    	    hrtime_t timeout = gethrtime() + (hrtime_t)3000000000;
    	    while( (REGR(RBBM_STATUS) & GUI_ACTIVE) && 
				gethrtime() < timeout )
        	yield();

            if( (REGR(RBBM_STATUS) & GUI_ACTIVE) != 0 )
      		efb_reset_engine(pEFB) ;
	}
}


