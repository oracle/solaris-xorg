/*
 * Copyright (c) 2000, 2004, Oracle and/or its affiliates. All rights reserved.
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

#ifndef _GFX_LIST_UTIL_H
#define	_GFX_LIST_UTIL_H

/*
 * gfx_list_util.h: declarations for generic list processing routines.
 */


typedef struct {
    int                  label;
    char                *name;
} OWConfigTableEntry;

typedef struct {
	int        alloc_type;
	int        max_len_bytes;
	int       *data;
} OWConfigLabelList;

#define OWCONFIG_LABEL_LIST_ALLOC_STATIC 0
#define OWCONFIG_LABEL_LIST_ALLOC_MALLOC 1
#define OWCONFIG_LABEL_LIST_END_LABEL    0

extern void                ow_table_init        (void**, OWConfigTableEntry*);
extern OWConfigTableEntry* ow_table_find        (void**, char*);
extern int                 ow_search_label_list (int, OWConfigLabelList*);
extern int                 ow_extend_label_list (int, OWConfigLabelList*);
#endif /* _GFX_LIST_UTIL_H */

