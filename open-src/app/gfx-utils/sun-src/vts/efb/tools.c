/*
 * Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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

#include "libvtsSUNWefb.h"	/* Common VTS library definitions */
#include "efb.h"


int
efb_map_mem(
    register return_packet *const rp,
    register int const test)
{
	register int const pagesize = getpagesize();

	if (efb_get_pci_info() != 0) {
		gfx_vts_set_message(rp, 1, test, "get pci info failed");
		return (-1);
	}

	/*
	 * Map MMIO
	 */
	efb_info.efb_mmio_size = (efb_info.efb_mmio_size + pagesize - 1) /
	    pagesize * pagesize;

	if (efb_map_mmio() != 0) {
		gfx_vts_set_message(rp, 1, test, "map MMIO failed");
		return (-1);
	}

	switch (efb_info.efb_device) {
	case 20825:
	case 20830:
	case 23396:
		efb_info.efb_fb_size = REGR(RADEON_CONFIG_MEMSIZE);
		break;
	default:
		efb_info.efb_fb_size = REGR(R600_CONFIG_MEMSIZE);
		break;
	}
	efb_info.efb_fb_size = (efb_info.efb_fb_size + (pagesize - 1)) /
	    pagesize * pagesize;

	/*
	 * Map framebuffer
	 */

	if (efb_map_fb() != 0) {
		gfx_vts_set_message(rp, 1, test, "map framebuffer failed");
		return (-1);
	}

	return (0);
}

int
efb_get_pci_info(
    void)
{
	struct gfx_pci_cfg pciconfig;
	int i;
	uint_t bar;
	uint_t bar_hi;
	offset_t mem_base[6];
	offset_t io_base[6];
	int type[6];

	if (ioctl(efb_info.efb_fd, GFX_IOCTL_GET_PCI_CONFIG,
	    &pciconfig) != 0) {
		return (-1);
	}

	efb_info.efb_vendor = pciconfig.VendorID;
	efb_info.efb_device = pciconfig.DeviceID;

	for (i = 0; i < 6; i++) {
		type[i] = 0;
		mem_base[i] = 0;
		io_base[i] = 0;
	}

	for (i = 0; i < 6; i++) {
		bar = pciconfig.bar[i];
		if (bar != 0) {
			if (bar & PCI_MAP_IO) {
				io_base[i] = PCIGETIO(bar);
				type[i] = bar & PCI_MAP_IO_ATTR_MASK;
			} else {
				type[i] = bar & PCI_MAP_MEMORY_ATTR_MASK;
				mem_base[i] = PCIGETMEMORY(bar);
				if (PCI_MAP_IS64BITMEM(bar)) {
					if (i == 5) {
						mem_base[i] = 0;
					} else {
						bar_hi = pciconfig.bar[i+1];
						mem_base[i] |=
						    ((offset_t)bar_hi << 32);
						++i;
					}
				}
			}
		}
	}

	efb_info.efb_fb_addr = mem_base[0] & 0xfff00000;
	efb_info.efb_fb_size = 0;

	efb_info.efb_mmio_addr = mem_base[2] & 0xffff0000;
	efb_info.efb_mmio_size = 1 << EFB_REG_SIZE_LOG2;

	if (gfx_vts_debug_mask & VTS_DEBUG) {
		printf("efb_vendor = 0x%04x, efb_device = 0x%04x\n",
		    efb_info.efb_vendor, efb_info.efb_device);
		printf("efb_fb_addr 0x%llx, efb_fb_size 0x%lx\n",
		    (unsigned long long)efb_info.efb_fb_addr,
		    (unsigned long)efb_info.efb_fb_size);
		printf("efb_mmio_addr 0x%llx, efb_mmio_size 0x%lx\n",
		    (unsigned long long)efb_info.efb_mmio_addr,
		    (unsigned long)efb_info.efb_mmio_size);
	}

	return (0);
}

int
efb_map_mmio(
    void)
{
	register void *ptr;

	if (efb_info.efb_mmio_ptr == NULL) {
		ptr = mmap(NULL, efb_info.efb_mmio_size,
		    PROT_READ | PROT_WRITE, MAP_SHARED,
		    efb_info.efb_fd, efb_info.efb_mmio_addr);

		if (ptr == MAP_FAILED)
			return (-1);
	}

	efb_info.efb_mmio_ptr = (uchar_t *)ptr;

	if (gfx_vts_debug_mask & VTS_DEBUG)
		printf("efb_mmio_ptr = 0x%llx\n",
		    (unsigned long long)efb_info.efb_mmio_ptr);

	return (0);
}

int
efb_map_fb(
    void)
{
	register void *ptr;

	if (efb_info.efb_fb_ptr == NULL) {
		ptr = mmap(NULL, efb_info.efb_fb_size,
		    PROT_READ | PROT_WRITE, MAP_SHARED,
		    efb_info.efb_fd, efb_info.efb_fb_addr);

		if (ptr == MAP_FAILED)
			return (-1);

		efb_info.efb_fb_ptr = (uchar_t *)ptr;
	}

	if (gfx_vts_debug_mask & VTS_DEBUG)
		printf("efb_fb_ptr = 0x%llx\n",
		    (unsigned long long)efb_info.efb_fb_ptr);

	return (0);
}


int
efb_init_info(
    register return_packet *const rp,
    register int const test)
{
	register uint32_t crtc_h_total_disp;
	register uint32_t crtc_v_total_disp;
	register uint32_t crtc_gen_cntl;
	register uint32_t crtc_pitch;
	register uint_t width;
	register uint_t height;
	register uint_t depth;
	register uint_t pixelsize;
	register uint_t pitch;

	/* Get the gen cntl */

	crtc_gen_cntl = REGR(CRTC_GEN_CNTL);

	/* Get the horizontal total display end */

	crtc_h_total_disp = REGR(CRTC_H_TOTAL_DISP);

	/* Get the vertical total display end */

	crtc_v_total_disp = REGR(CRTC_V_TOTAL_DISP);

	/* Get the pitch */

	crtc_pitch = REGR(CRTC_PITCH);

	/* Compute the width. */

	width = (((crtc_h_total_disp & CRTC_H_TOTAL_DISP__CRTC_H_DISP_MASK) >>
	    CRTC_H_TOTAL_DISP__CRTC_H_DISP__SHIFT) + 1) * 8;

	/* Compute the height. */

	height = ((crtc_v_total_disp & CRTC_V_TOTAL_DISP__CRTC_V_DISP_MASK) >>
	    CRTC_V_TOTAL_DISP__CRTC_V_DISP__SHIFT) + 1;

	if (crtc_gen_cntl & CRTC_GEN_CNTL__CRTC_INTERLACE_EN_MASK)
		height *= 2;

	/* Compute the pitch */

	pitch = crtc_pitch & CRTC_PITCH__CRTC_PITCH_MASK;

	/* Compute the depth. */

	switch ((crtc_gen_cntl & CRTC_GEN_CNTL__CRTC_PIX_WIDTH_MASK) >>
	    CRTC_GEN_CNTL__CRTC_PIX_WIDTH__SHIFT) {

	case 2:
		depth = 8;
		pixelsize = 1;
		pitch *= 8;
		break;

	case 3:
		depth = 15;
		pixelsize = 2;
		pitch *= 16;
		break;

	case 4:
		depth = 16;
		pixelsize = 2;
		pitch *= 16;
		break;

	case 5:
		depth = 24;
		pixelsize = 3;
		pitch *= 24;
		break;

	case 6:
		depth = 32;
		pixelsize = 4;
		pitch *= 32;
		break;

	default:
		gfx_vts_set_message(rp, 1, test, "unsupported depth");
		return (-1);
	}

	efb_info.efb_width = width;
	efb_info.efb_height = height;
	efb_info.efb_depth = depth;
	efb_info.efb_pixelsize = pixelsize;
	efb_info.efb_linesize = pitch;

	if (gfx_vts_debug_mask & VTS_DEBUG) {
		printf("width=%d height=%d depth=%d pitch=%d\n",
		    efb_info.efb_width, efb_info.efb_height,
		    efb_info.efb_depth, efb_info.efb_linesize);
	}
	return (0);
}

int
efb_unmap_mem(
    register return_packet *const rp,
    register int const test)
{
	if (efb_unmap_fb() != 0) {
		gfx_vts_set_message(rp, 1, test, "unmap framebuffer failed");
		return (-1);
	}

	if (efb_unmap_mmio() != 0) {
		gfx_vts_set_message(rp, 1, test, "unmap MMIO failed");
		return (-1);
	}

	return (0);
}


int
efb_unmap_fb(
    void)
{
	register int status;

	if (efb_info.efb_fb_ptr == NULL)
		return (0);

	status = munmap((char *)efb_info.efb_fb_ptr, efb_info.efb_fb_size);
	efb_info.efb_fb_ptr = NULL;

	return (status);
}


int
efb_unmap_mmio(
    void)
{
	register int status;

	if (efb_info.efb_mmio_ptr == NULL)
		return (0);

	status = munmap((char *)efb_info.efb_mmio_ptr,
	    efb_info.efb_mmio_size);
	efb_info.efb_mmio_ptr = NULL;

	return (status);
}

int
efb_init_graphics(void)
{
	uint_t pitch_offset;
	uint_t pitch;
	uint_t offset;
	uint_t gmc_bpp;
	uint_t v;

	switch (efb_info.efb_depth) {
	case 8:
		gmc_bpp = GMC_DST_8BPP;
		break;
	case 15:
		gmc_bpp = GMC_DST_15BPP;
		break;

	case 16:
		gmc_bpp = GMC_DST_16BPP;
		break;

	case 24:
		gmc_bpp = GMC_DST_24BPP;
		break;

	case 32:
		gmc_bpp = GMC_DST_32BPP;
		break;
	}

	offset = REGR(CRTC_OFFSET) & 0x7ffffff;
	pitch = REGR(CRTC_PITCH) & 0x7ff;

	pitch = pitch * 8;	/* was in groups of 8 pixels */

	pitch_offset =
	    ((pitch * efb_info.efb_pixelsize / 64) << 22) |
	    (offset / 1024);

	/*
	 * Initialize GUI engine
	 */
	if (!efb_wait_idle())
		return (0);

	if (!efb_wait_fifo(5))
		return (0);

	REGW(DEFAULT_PITCH_OFFSET, pitch_offset);
	REGW(RADEON_DST_PITCH_OFFSET, pitch_offset);
	REGW(RADEON_SRC_PITCH_OFFSET, pitch_offset);

	REGW(DEFAULT_SC_BOTTOM_RIGHT,
	    (efb_info.efb_height << 16) | (efb_info.efb_width));

	v = (GMC_SRC_PITCH_OFFSET_DEFAULT |
	    GMC_DST_PITCH_OFFSET_LEAVE |
	    GMC_SRC_CLIP_DEFAULT |
	    GMC_DST_CLIP_DEFAULT |
	    GMC_BRUSH_SOLIDCOLOR |
	    gmc_bpp |
	    GMC_SRC_DSTCOLOR |
	    RADEON_ROP3_P |
	    GMC_WRITE_MASK_LEAVE);

	REGW(RADEON_DP_GUI_MASTER_CNTL, v);

#ifdef DEBUG
	printf("v=0x%x\n", v);
#endif
	return (1);
}


void
efb_save_palet(
    void)
{
	register uint_t coloron;
	register uint_t const save_palette_index =
	    REGR(RADEON_PALETTE_INDEX);

	REGW(RADEON_PALETTE_INDEX, 0);

	for (coloron = 0; coloron < 256; coloron++)
		efb_info.efb_palet[coloron] = REGR(RADEON_PALETTE_30_DATA);

	REGW(RADEON_PALETTE_INDEX, save_palette_index);
}


int
efb_set_palet(
    void)
{
	register uint_t coloron;
	register uint_t save_palette_index;
	uint_t new_red;
	uint_t new_green;
	uint_t new_blue;
	uint_t new_palet[256];

	switch (efb_info.efb_depth) {
	case 8:	/* 3, 3, 2 */
		for (coloron = 0; coloron < 256; coloron++) {
			new_red = ((coloron >> 5) & 0x7) * 1023 / 7;
			new_green = ((coloron >> 2) & 0x7) * 1023 / 7;
			new_blue = (coloron & 0x3) * 1023 / 3;
			new_palet[coloron] =
			    (new_red <<
			    PALETTE_30_DATA__PALETTE_DATA_R__SHIFT) |
			    (new_green <<
			    PALETTE_30_DATA__PALETTE_DATA_G__SHIFT) |
			    (new_blue <<
			    PALETTE_30_DATA__PALETTE_DATA_B__SHIFT);
		}
		break;

	case 15:	/* 5, 5, 5 */
		for (coloron = 0; coloron < 256; coloron++) {
			new_red = (coloron / 8) * 1023 / 31;
			new_green = (coloron / 8) * 1023 / 31;
			new_blue = (coloron / 8) * 1023 / 31;
			new_palet[coloron] =
			    (new_red <<
			    PALETTE_30_DATA__PALETTE_DATA_R__SHIFT) |
			    (new_green <<
			    PALETTE_30_DATA__PALETTE_DATA_G__SHIFT) |
			    (new_blue <<
			    PALETTE_30_DATA__PALETTE_DATA_B__SHIFT);
		}
		break;

	case 16:	/* 5, 6, 5 */
		for (coloron = 0; coloron < 256; coloron++) {
			new_red = (coloron / 8) * 1023 / 31;
			new_green = (coloron / 4) * 1023 / 63;
			new_blue = (coloron / 8) * 1023 / 31;
			new_palet[coloron] =
			    (new_red <<
			    PALETTE_30_DATA__PALETTE_DATA_R__SHIFT) |
			    (new_green <<
			    PALETTE_30_DATA__PALETTE_DATA_G__SHIFT) |
			    (new_blue <<
			    PALETTE_30_DATA__PALETTE_DATA_B__SHIFT);
		}
		break;

	default:	/* 8, 8, 8 */
		for (coloron = 0; coloron < 256; coloron++) {
			new_red = (coloron * 1023) / 255;
			new_green = (coloron * 1023) / 255;
			new_blue = (coloron * 1023) / 255;
			new_palet[coloron] =
			    (new_red <<
			    PALETTE_30_DATA__PALETTE_DATA_R__SHIFT) |
			    (new_green <<
			    PALETTE_30_DATA__PALETTE_DATA_G__SHIFT) |
			    (new_blue <<
			    PALETTE_30_DATA__PALETTE_DATA_B__SHIFT);
		}
		break;
	}

	/* Don't set the palet if it matches what we will set. */

	for (coloron = 0; coloron < 256; coloron++) {
		if ((efb_info.efb_palet[coloron] &
		    (PALETTE_30_DATA__PALETTE_DATA_R_MASK |
		    PALETTE_30_DATA__PALETTE_DATA_G_MASK |
		    PALETTE_30_DATA__PALETTE_DATA_B_MASK)) !=
		    (new_palet[coloron] &
		    (PALETTE_30_DATA__PALETTE_DATA_R_MASK |
		    PALETTE_30_DATA__PALETTE_DATA_G_MASK |
		    PALETTE_30_DATA__PALETTE_DATA_B_MASK)))
			break;
	}

	if (coloron == 256)
		return (0);

	efb_info.efb_palet_changed = 1;
	save_palette_index = REGR(RADEON_PALETTE_INDEX);

	REGW(RADEON_PALETTE_INDEX, 0);

	for (coloron = 0; coloron < 256; coloron++)
		REGW(RADEON_PALETTE_30_DATA, new_palet[coloron]);

	REGW(RADEON_PALETTE_INDEX, save_palette_index);
	return (1);
}

int
efb_restore_palet(
    void)
{
	register uint_t coloron;
	register uint_t save_palette_index;

	if (!efb_info.efb_palet_changed)
		return (0);

	save_palette_index = REGR(RADEON_PALETTE_INDEX);

	REGW(RADEON_PALETTE_INDEX, 0);

	for (coloron = 0; coloron < 256; coloron++)
		REGW(RADEON_PALETTE_30_DATA, efb_info.efb_palet[coloron]);

	REGW(RADEON_PALETTE_INDEX, save_palette_index);

	efb_info.efb_palet_changed = 0;
	return (1);
}


uint_t
efb_color(
    register uint_t const red,
    register uint_t const green,
    register uint_t const blue)
{
	register uint_t value;

	switch (efb_info.efb_depth) {
	case 8:	/* 3, 3, 2 */
		value = ((red >> 5) & 0x7) << 5;
		value |= ((green >> 5) & 0x7) << 2;
		value |= (blue >> 6) & 0x3;
		break;

	case 15:	/* 5, 5, 5 */
		value = ((red >> 3) & 0x1f) << 10;
		value |= ((green >> 3) & 0x1f) << 5;
		value |= (blue >> 3) & 0x1f;
		break;

	case 16:	/* 5, 6, 5 */
		value = ((red >> 3) & 0x1f) << 11;
		value |= ((green >> 2) & 0x3f) << 5;
		value |= (blue >> 3) & 0x1f;
		break;

	default:	/* 8, 8, 8 */
		value = (red & 0xff) << 16;
		value |= (green & 0xff) << 8;
		value |= blue & 0xff;
		break;
	}

	return (value);
}


int
efb_fill_solid_rect(
    register uint_t const x1,
    register uint_t const y1,
    register uint_t const x2,
    register uint_t const y2,
    register uint_t const fg)
{
	register int width;
	register int height;

	width  = x2 - x1;
	height = y2 - y1;

	if ((width <= 0) || (height <= 0)) {
#ifdef DEBUG
		printf("x1=%d x2=%d y1=%d y2=%d\n", x1, x2, y1, y2);
#endif
		return (0);
	}

	if (!efb_wait_fifo(5))
		return (0);

	REGW(RADEON_DP_WRITE_MASK, 0xffffffff);
	REGW(RADEON_DP_CNTL,
	    (RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM));

	REGW(RADEON_DP_BRUSH_FRGD_CLR, fg);
	REGW(DST_Y_X,  (x1 << DST_Y_X__DST_X__SHIFT) |
	    (y1 << DST_Y_X__DST_Y__SHIFT));
	REGW(DST_WIDTH_HEIGHT,
	    (height << DST_WIDTH_HEIGHT__DST_HEIGHT__SHIFT) |
	    (width << DST_WIDTH_HEIGHT__DST_WIDTH__SHIFT));

	return (1);
}

int
efb_draw_solid_line(
    register uint_t const x1,
    register uint_t const y1,
    register uint_t const x2,
    register uint_t const y2,
    register uint_t const fg)
{
	if (!efb_wait_fifo(5))
		return (0);

	REGW(RADEON_DP_WRITE_MASK, 0xffffffff);
	REGW(RADEON_DP_CNTL,
	    (RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM));

	REGW(RADEON_DP_BRUSH_FRGD_CLR, fg);
	REGW(DST_LINE_START,
	    (x1 << DST_LINE_START__DST_START_X__SHIFT) |
	    (y1 << DST_LINE_START__DST_START_Y__SHIFT));
	REGW(DST_LINE_END,
	    (x2 << DST_LINE_END__DST_END_X__SHIFT) |
	    (y2 << DST_LINE_END__DST_END_Y__SHIFT));

	return (1);
}	/* line() */

int
efb_flush_pixel_cache(
    void)
{
	register int i;

	/* initiate flush */
	REGW(RADEON_RB2D_DSTCACHE_CTLSTAT,
	    REGR(RADEON_RB2D_DSTCACHE_CTLSTAT) | 0xf);

	/* check for completion but limit looping to 16384 reads */
	for (i = 16384; i > 0; i--) {
		if ((REGR(RADEON_RB2D_DSTCACHE_CTLSTAT) &
		    (uint_t)RADEON_RB2D_DC_BUSY) != (uint_t)RADEON_RB2D_DC_BUSY)
			break;
	}
	return ((REGR(RADEON_RB2D_DSTCACHE_CTLSTAT) &
	    (uint_t)RADEON_RB2D_DC_BUSY) != (uint_t)RADEON_RB2D_DC_BUSY);
}

void
efb_reset_engine(
    void)
{
	register uint_t save_genresetcntl;
	register uint_t save_clockcntlindex;
	static ulong_t term_count;

	efb_flush_pixel_cache();

	save_clockcntlindex = REGR(RADEON_CLOCK_CNTL_INDEX);

	/* save GEN_RESET_CNTL register */
	save_genresetcntl = REGR(RADEON_DISP_MISC_CNTL);

	/* reset by setting bit, add read delay, then clear bit, */
	/* add read delay */
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl |
	    RADEON_DISP_MISC_CNTL__SOFT_RESET_GRPH_PP);
	REGR(RADEON_DISP_MISC_CNTL);
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl &
	    ~(RADEON_DISP_MISC_CNTL__SOFT_RESET_GRPH_PP));
	REGR(RADEON_DISP_MISC_CNTL);

	/* restore the two registers we changed */
	REGW(RADEON_CLOCK_CNTL_INDEX, save_clockcntlindex);
	REGW(RADEON_DISP_MISC_CNTL, save_genresetcntl);

	term_count++;	/* for monitoring engine hangs */
}

int
efb_wait_fifo(
    register int const c)
{
	register int limit;
	register hrtime_t timeout;

	/* First a short loop, just in case fifo clears out quickly */
	for (limit = 100; limit >= 0; limit--) {
		if ((REGR(RBBM_STATUS) &
		    RBBM_STATUS__CMDFIFO_AVAIL_MASK) >= c)
			break;
	}

	if ((REGR(RBBM_STATUS) & RBBM_STATUS__CMDFIFO_AVAIL_MASK) < c) {
		timeout = gethrtime() + 3 * (hrtime_t)1000000000;
		while (gethrtime() < timeout) {
			if ((REGR(RBBM_STATUS) &
			    RBBM_STATUS__CMDFIFO_AVAIL_MASK) >= c)
				break;
			yield();
		}
		if ((REGR(RBBM_STATUS) &
		    RBBM_STATUS__CMDFIFO_AVAIL_MASK) < c)
			efb_reset_engine();
	}
	return ((REGR(RBBM_STATUS) &
	    RBBM_STATUS__CMDFIFO_AVAIL_MASK) >= c);
}

int
efb_wait_idle(
    void)
{
	register int limit;
	register hrtime_t timeout;

	efb_wait_fifo(64);

	for (limit = 10000; limit > 0; limit--) {
		if (!(REGR(RBBM_STATUS) & GUI_ACTIVE))
			break;
	}

	if (REGR(RBBM_STATUS) & GUI_ACTIVE) {
		timeout = gethrtime() + 3 * (hrtime_t)1000000000;
		while (gethrtime() < timeout) {
			if (!(REGR(RBBM_STATUS) & GUI_ACTIVE))
				break;
			yield();
		}

		if ((REGR(RBBM_STATUS) & GUI_ACTIVE) != 0)
			efb_reset_engine();
	}
	return ((REGR(RBBM_STATUS) & GUI_ACTIVE) == 0);
}
