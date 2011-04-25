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

/*
 * efb_prconf - Display current efb hardware configuration
 */

#include <sys/int_types.h>	/* uint8_t */
#include <stdio.h>		/* printf(), puts() */
#include <stdlib.h>		/* free() */

#include "xf86Parser.h"		/* Public function, etc. declarations */

#include "sun_edid.h"		/* EDID data parsing */

#include "fbc.h"		/* Common fbconf_xorg(1M) definitions */
#include "fbc_dev.h"		/* Identify the graphics device (-dev opt) */
#include "fbc_error.h"		/* Error reporting */
#include "fbc_prconf.h"		/* Display current hardware configuration */
#include "fbc_properties.h"	/* fbconf_xorg(1M) program properties */
#include "fbc_query_device.h"	/* Query a frame buffer device */

#include "efb_prconf.h"		/* Display current efb hardware config */


/*
 * efb_prconf_stream()
 *
 *    Display the monitor and resolution information for the indicated
 *    video stream (stream_bit), unless we determine that this stream
 *    isn't in the set of streams (stream_set) the user wants displayed.
 */

static
void
efb_prconf_stream(
	int		device_fd,	/* Device file descriptor number */
	int		stream_index,	/* Video stream index (zero-based) */
	fbc_mode_elem_t	*mode_list)	/* Modes from Monitor section of cfg */
{
	uint8_t		*edid_data;	/* EDID Base block, etc. */
	size_t		edid_length;	/* EDID data length */

	/*
	 * Get and display the EDID data for this stream's display device
	 */
	fbc_get_edid_data(device_fd, stream_index, &edid_data, &edid_length);
	fbc_prconf_edid(edid_data, edid_length, mode_list);
	free(edid_data);

	/*
	 * Display the current video mode setting
	 */
	fbc_prconf_cur_mode(device_fd);

}	/* efb_prconf_stream() */


/*
 * efb_prconf()
 *
 *    Display the current hardware configuration (-prconf).
 */

void
efb_prconf(
	const fbc_dev_t *device,	/* Frame buffer device info (-dev) */
	fbc_varient_t	*fbvar,		/* fbconf_xorg(1M) varient data */
	XF86ConfigPtr	configIR)	/* Config file Internal Rep */
{
	fbc_mode_elem_t	*mode_list;	/* Modes from Monitor section of cfg */
	int		stream_index;	/* Video stream index (zero-based) */

	/*
	 * Display the frame buffer model name and part number
	 */
	fbc_prconf_model(device->fd);

	/*
	 * Get the Modes declared by the active Monitor section of the config
	 *
	 *    This is an "unintrusive" linked list.  Each list element
	 *    points to, rather than contains, a ModeLine /
	 *    Mode-EndMode entry.  The mode entries are independent of
	 *    the list.
	 */
	mode_list = fbc_get_mode_list(configIR,
					fbvar->active.monitor_sectn,
					device->type);

	/*
	 * Display the monitor info for each of the indicated streams (-dev)
	 */
	printf("\nMonitor/Resolution Information:\n");

	for (stream_index = device->stream_lo;
	    stream_index <= device->stream_hi;
	    stream_index += 1) {
		printf("Monitor %d:\n", stream_index + 1);
		efb_prconf_stream(device->fd, stream_index, mode_list);
	}

	fbc_free_mode_list(mode_list);	/* Free the list but not the Modes */

}	/* efb_prconf() */


/* End of efb_prconf.c */
