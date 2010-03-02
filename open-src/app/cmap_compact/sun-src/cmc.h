/*
*
* Copyright 1990 Sun Microsystems, Inc.  All rights reserved.
* Use is subject to license terms.
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
** Definitions for all Colormap compacting operations
*/

#ifndef CMC_INCLUDE
#define CMC_INCLUDE


/*
** External Data
*/

extern char	*display_name;
extern char	*basename;
extern int	warn;


/*
** External Functions
*/

extern void	fatal_error();
extern void	cmc_save();
extern void	cmc_init();
extern void	cmc_show();
extern void	cmc_discard();
extern void	cmc_dealloc();
extern void	cmc_NeWSinit();


#endif  !CMC_INCLUDE
