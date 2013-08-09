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

/*
 * efb_test_open()
 *
 *    This test will open the device, read and write some registers
 *    after mmaping in the register and frame buffer spaces.
 */

return_packet *
efb_test_open(
    register int const fd)
{
	static return_packet rp;
	int rc = 0;
	struct vis_identifier vis_identifier;

	memset(&rp, 0, sizeof (return_packet));

	if (gfx_vts_check_fd(fd, &rp))
		return (&rp);

	TraceMessage(VTS_TEST_STATUS, "efb_test_open", "check_fd passed.\n");

	/* vis identifier will do this */
	rc = ioctl(fd, VIS_GETIDENTIFIER, &vis_identifier);

	TraceMessage(VTS_TEST_STATUS, "efb_test_open", "rc = %d\n", rc);

	if (rc != 0) {
		gfx_vts_set_message(&rp, 1, GRAPHICS_ERR_OPEN, NULL);
		return (&rp);
	}

	if (strncmp(vis_identifier.name, "SUNWefb", 7) != 0 &&
	    strncmp(vis_identifier.name, "ORCLefb", 7) != 0) {
		gfx_vts_set_message(&rp, 1, GRAPHICS_ERR_OPEN, NULL);
		return (&rp);
	}

	efb_block_signals();

	efb_lock_display();

	map_me(&rp, fd);

	efb_unlock_display();

	efb_restore_signals();

	TraceMessage(VTS_DEBUG, "efb_test_open", "Open completed OK\n");

	return (&rp);

}	/* efb_test_open() */


/*
 * map_me()
 */

int
map_me(
    register return_packet *const rp,
    register int const fd)
{
	memset(&efb_info, 0, sizeof (efb_info));
	efb_info.efb_fd = fd;

	if (efb_map_mem(rp, GRAPHICS_ERR_OPEN) != 0)
		return (-1);

	if (efb_test_semaphore(rp, GRAPHICS_ERR_OPEN) != 0) {
		efb_unmap_mem(NULL, GRAPHICS_ERR_OPEN);
		return (-1);
	}

	/*
	 * Unmap the registers & frame buffers memory
	 */
	if (efb_unmap_mem(rp, GRAPHICS_ERR_OPEN) != 0)
		return (-1);

	return (0);
}	/* map_me() */

#define	RADEON_SW_SEMAPHORE_MASK	0xffff
#define	SEMAPHORE_XOR_VALUE		0x5555

int
efb_test_semaphore(
    register return_packet *const rp,
    register int const test)
{
	register uint_t save_semaphore;
	register uint_t new_semaphore;

	/*
	 * Test the software bits of the semaphore register.
	 * Complement the bits, and see if they read back
	 * complemented.
	 */

	/* Save the original semaphore. */

	save_semaphore = REGR(RADEON_SW_SEMAPHORE);

	/* Write the semaphore with the software bits complemented. */
	REGW(RADEON_SW_SEMAPHORE,
	    save_semaphore ^ SEMAPHORE_XOR_VALUE);

	/* Get the new semaphore. */

	new_semaphore = REGR(RADEON_SW_SEMAPHORE);

	/* Restore the old semaphore. */

	REGW(RADEON_SW_SEMAPHORE, save_semaphore);

	/* Check if the software bits did complement. */

	if ((save_semaphore & RADEON_SW_SEMAPHORE_MASK) !=
	    ((new_semaphore ^ SEMAPHORE_XOR_VALUE) &
	    RADEON_SW_SEMAPHORE_MASK)) {
		printf("semaphore 0x%08x 0x%08x 0x%08x\n",
		    save_semaphore, new_semaphore,
		    save_semaphore ^ SEMAPHORE_XOR_VALUE);

		gfx_vts_set_message(rp, 1, test, "semaphore test failed");
		return (0);
	}
	return (1);
}

/* End of mapper.c */
