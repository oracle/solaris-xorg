/*
* Copyright (c) 1990, Oracle and/or its affiliates. All rights reserved.
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
*
*/


/*
** Definitions shared between several Workspace Color operations
*/

#ifndef CMCUTIL_INCLUDE
#define CMCUTIL_INCLUDE

/*
** Symbols
*/

#define COUNT_PROP_NAME		"XA_COMPACTED_COLORS_COUNT"
#define RETAIN_PROP_NAME	"XA_COMPACTED_COLORS_XID"

/* 
** Note: if you change these, you must also change it in 
** server/cscript/cs_cmap.c.
*/
#define COMPACTED_COLORS_FILE	".owcolors"
#define CMC_MAGIC		0xb0f0
#define CMC_VERSION		0



/*
** Macros
*/

#define WHITE(c)\
	((c)->red == (c)->green && \
	 (c)->red == (c)->blue  && \
	 (c)->red == 0xffff)

#define BLACK(c)\
	((c)->red == (c)->green && \
	 (c)->red == (c)->blue  && \
	 (c)->red == 0x0)

/*
** Types
*/

typedef unsigned long Pixel;


/*
** External Functions
*/

extern void		setprogram();
extern void		fatal_error();
extern Display		*open_display();
extern int		dynamic_indexed_default_visual();
extern char		*comp_colors_filename();
extern int		cmc_write();
extern int		cmc_read();
extern void		cmc_header_write();
extern void		cmc_header_test();
extern void		resource_preserve();
extern void		resource_discard();
extern void		prop_update();

#endif !CMCUTIL_INCLUDE
