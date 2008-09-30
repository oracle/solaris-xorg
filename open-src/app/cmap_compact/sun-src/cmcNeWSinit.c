/*
* Copyright 1990 Sun Microsystems, Inc.  All rights reserved.
* Use subject to license terms.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, and/or sell copies of the Software, and to permit persons
* to whom the Software is furnished to do so, provided that the above
* copyright notice(s) and this permission notice appear in all copies of
* the Software and that both the above copyright notice(s) and this
* permission notice appear in supporting documentation.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
* INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
* NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
* WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* Except as contained in this notice, the name of a copyright holder
* shall not be used in advertising or otherwise to promote the sale, use
* or other dealings in this Software without prior written authorization
* of the copyright holder.
*/

#pragma ident "@(#)cmcNeWSinit.c	35.4 08/09/30 CMAP_COMPACT SMI"

#include <stdio.h>
#include <X11/Xlib.h>
#include "cmc.h"
#include "cmcutil.h"


/*
** For each screen, generates NeWS PostScript commands on stdio
** capable of doing X11/NeWS-specific initialization when executed.
**
** Output is of the form:
**
** 	<scr_num> <ncolors> cmap_compact_rearrange_scr
** 	<scr_num> <ncolors> cmap_compact_rearrange_scr
**	...
**
** where <scr_num> is the screen index of each screen,
** <ncolors> is the number of saved colors, and 'cmap_compact_rearrange_scr'
** is a NeWS routine which performs rearrangement of various predefined
** colors to further reduce colormap flashing with other X11/NeWS
** programs.
*/

void
cmc_NeWSinit ()

{
	char	*filename;
	FILE	*f;

	filename = comp_colors_filename(basename);
	if ((f = fopen(filename, "r")) == NULL)
		exit(0);

	/* Check magic number and version */
	cmc_header_test(f);

	for (;;) {
		int	scr_num;
		int	ncolors;
		XColor	*colors;

		if (!cmc_read(f, &scr_num, &ncolors, &colors))
			break;

		printf("%d %d cmap_compact_rearrange_scr\n", scr_num, ncolors);

		free((char *)colors);
	}
}


