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

#include "libvtsSUNWefb.h"

/*
 * efb_test_chip()
 *
 *    Test Chip, functional tests.
 */

return_packet *
efb_test_chip(
    register int const fd)
{
	static return_packet rp;

	memset(&rp, 0, sizeof (return_packet));

	if (gfx_vts_debug_mask & GRAPHICS_VTS_CHIP_OFF)
		return (&rp);

	TraceMessage(VTS_DEBUG, "efb_test_chip", "efb_test_chip running\n");

	efb_block_signals();

	efb_lock_display();

	chip_test(&rp, fd);

	efb_unlock_display();

	efb_restore_signals();

	TraceMessage(VTS_DEBUG, "efb_test_chip", "efb_test_chip completed\n");

	return (&rp);

}	/* efb_test_chip() */


int
chip_test(
    register return_packet *const rp,
    register int const fd)
{
	register uint_t black;
	register uint_t white;

	memset(&efb_info, 0, sizeof (efb_info));
	efb_info.efb_fd = fd;

	/*
	 * map the registers & frame buffers memory
	 */
	if (efb_map_mem(rp, GRAPHICS_ERR_CHIP) != 0)
		return (-1);

	if (efb_init_info(rp, GRAPHICS_ERR_CHIP) != 0) {
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	if (!efb_init_graphics()) {
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	efb_save_palet();
	efb_set_palet();

	/*
	 * Clear screen black
	 */
	black = efb_color(0x00, 0x00, 0x00);
	if (!efb_fill_solid_rect(0, 0,
	    efb_info.efb_width, efb_info.efb_height, black)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	if (!efb_wait_idle()) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	efb_flush_pixel_cache();

	/*
	 * line test
	 */
	if (!draw_lines(efb_info.efb_width, efb_info.efb_height)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	if (!efb_wait_idle()) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	efb_flush_pixel_cache();

	if (efb_sleep(2)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	/*
	 * fill rectangle test
	 */
	if (!draw_cascaded_box(efb_info.efb_width, efb_info.efb_height)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	if (!efb_wait_idle()) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	efb_flush_pixel_cache();

	if (efb_sleep(2)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	/* Clear screen white */

	white = efb_color(0xff, 0xff, 0xff);

	/*
	 * Clear screen
	 */
	if (!efb_fill_solid_rect(0, 0,
	    efb_info.efb_width, efb_info.efb_height, white)) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	if (!efb_wait_idle()) {
		efb_restore_palet();
		efb_unmap_mem(NULL, GRAPHICS_ERR_CHIP);
		return (-1);
	}

	efb_flush_pixel_cache();

	efb_sleep(2);

	efb_restore_palet();

	if (efb_unmap_mem(rp, GRAPHICS_ERR_CHIP) != 0)
		return (-1);

	return (0);
}	/* chip_test() */


int
draw_lines(
    register uint_t const width,
    register uint_t const height)
{
	register uint_t x1;
	register uint_t y1;
	register uint_t x2;
	register uint_t y2;
	register uint_t color;
	register uint_t lineon;
	register uint_t const numlines = 128;

	for (lineon = 0; lineon < numlines; lineon++) {
		color = efb_color(0xaf, lineon, lineon);

		x1 = (uint_t)((width * lineon) / numlines);
		x2 = x1;
		y1 = 0;
		y2 = height;

		if (!efb_draw_solid_line(x1, y1, x2, y2, color))
			return (0);
	}

	for (lineon = 0; lineon < numlines; lineon++) {
		color = efb_color(0xaf, lineon, lineon);

		x1 = 0;
		x2 = width;
		y1 = (uint_t)((height * lineon) / numlines);
		y2 = y1;

		if (!efb_draw_solid_line(x1, y1, x2, y2, color))
			return (0);
	}
	return (1);
}

int
draw_cascaded_box(
    register uint_t const width,
    register uint_t const height)
{
	register uint_t x1;
	register uint_t y1;
	register uint_t x2;
	register uint_t y2;
	register uint_t color;
	register uint_t recton;
	register uint_t const numrects = 256;

	for (recton = 0; recton < numrects; recton++) {

		x1 = (uint_t)((width * recton) / 512);
		x2 = width - x1;

		y1 = (uint_t)((height * recton) / 512);
		y2 = height - y1;

		color = efb_color(recton, recton, recton);

		if (!efb_fill_solid_rect(x1, y1, x2, y2, color))
			return (0);
	}

	return (1);
}

/* End of chip.c */
