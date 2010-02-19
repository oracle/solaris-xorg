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
*
*/

#include <stdio.h>
#include <X11/Xlib.h>
#include "cmc.h"
#include "cmcutil.h"


static void print_xcolors();


/*
** Print contents of workspace colors file in human readable form.
*/

void
cmc_show ()

{
	char	*filename;
	FILE	*f;

	filename = comp_colors_filename(basename);
	if ((f = fopen(filename, "r")) == NULL)
		fatal_error("cannot open file '%s' for reading\n", filename);

	/* Check magic number and version */
	cmc_header_test(f);

	for (;;) {
		int	scr_num;
		int	ncolors;
		XColor	*colors;

		if (!cmc_read(f, &scr_num, &ncolors, &colors))
			break;

		printf("Screen %s.%d\n", XDisplayName(display_name), scr_num);	
		printf("%d saved colors\n", ncolors);

		print_xcolors(colors, ncolors);
		printf("\n");

		free((char *)colors);
	}
}


static void
print_xcolors (colors, ncolors)
XColor	*colors;
int	ncolors;

{
	register XColor	*c;
	register int	i;
	register int	max = 0;
	char	      	format[50];
	int		ndigits = 4;
	int		planes;

	for (c = colors; c < colors + ncolors; c++) {
            planes = c->red | c->green | c->blue;
            for(i = 4; i > 1 && (planes&0xf)==0; i--) {
                c->red >>= 4;
                c->green >>= 4;
                c->blue >>= 4;
                planes >>= 4;
            }
            printf("#%0*x%0*x%0*x\n", i, c->red, i, c->green, i, c->blue);
	}
}	
