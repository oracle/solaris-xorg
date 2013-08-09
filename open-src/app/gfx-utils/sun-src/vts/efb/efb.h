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

#ifndef	_EFB_H
#define	_EFB_H

#include "radeon_reg.h"
#include "efb_reg.h"

#define	EFB_REG_SIZE_LOG2	18

#define	READ_MMIO_UINT(addr)		*((uint_t volatile *)(addr))
#define	WRITE_MMIO_UINT(addr, val)	*((uint_t volatile *)(addr)) = \
	(val)

#define	INREG(offset)		READ_MMIO_UINT(efb_info.efb_mmio_ptr + \
	(offset))
#define	REGW(offset, value)	WRITE_MMIO_UINT(efb_info.efb_mmio_ptr + \
	(offset), (value))
#define	REGR(offset)		READ_MMIO_UINT(efb_info.efb_mmio_ptr + \
	(offset))

#define	PCI_MAP_MEMORY			0x00000000
#define	PCI_MAP_IO			0x00000001

#define	PCI_MAP_MEMORY_TYPE		0x00000007
#define	PCI_MAP_IO_TYPE			0x00000003

#define	PCI_MAP_MEMORY_TYPE_32BIT	0x00000000
#define	PCI_MAP_MEMORY_TYPE_32BIT_1M	0x00000002
#define	PCI_MAP_MEMORY_TYPE_64BIT	0x00000004
#define	PCI_MAP_MEMORY_TYPE_MASK	0x00000006
#define	PCI_MAP_MEMORY_CACHABLE		0x00000008
#define	PCI_MAP_MEMORY_ATTR_MASK	0x0000000e
#define	PCI_MAP_MEMORY_ADDRESS_MASK	0xfffffff0

#define	PCI_MAP_IO_ATTR_MASK		0x00000003
#define	PCI_MAP_IS_IO(b)		((b) & PCI_MAP_IO)
#define	PCI_MAP_IO_ADDRESS_MASK		0xfffffffc

#define	PCIGETIO(b)			((b) & PCI_MAP_IO_ADDRESS_MASK)

#define	PCI_MAP_IS64BITMEM(b) \
	(((b) & PCI_MAP_MEMORY_TYPE) == PCI_MAP_MEMORY_TYPE_64BIT)

#define	PCIGETMEMORY(b)			((b) & PCI_MAP_MEMORY_ADDRESS_MASK)

#define	PCI_REGION_BASE(_pcidev, _b, _type)			\
	(((_type) == REGION_MEM) ? (_pcidev)->memBase[(_b)] :	\
	(_pcidev)->ioBase[(_b)])

#endif	/* _EFB_H */
