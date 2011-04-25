
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


#ifndef _LIBVTSSUNWEFB_H
#define	_LIBVTSSUNWEFB_H


#include "libvtsSUNWxfb.h"	/* Common VTS library definitions */


/* Test Open, initialization & read back; basic ability to talk to device */
return_packet *efb_test_open(int fd);

/* Test DMA, read/write via dma, memory and, chip tests will use DMA */
return_packet *efb_test_dma(int fd);

/* Test Memory, various address, data, and, random patterns */
return_packet *efb_test_memory(int fd);

/* Test Chip, functional tests */
return_packet *efb_test_chip(int fd);

void check4abort();
int get_processHandle(int, void **);

extern int backup_clut_init;
extern struct fbcmap backup_clut;
extern int myfd;


#endif	/* _LIBVTSSUNWEFB_H */


/* End of libvtsSUNWefb.h */
