###############################################################################
#
# hwc 1.x Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#


MACH:sh=uname -p

CPPFLAGS = -I. -D_KERNEL -D_SUNDDI -D_SYSCALL32

BUILD_i386_FLAGS =
BUILD_sparc_FLAGS = -D_SYSCALL32_IMPL \
		    -xchip=ultra -Wc,-xcode=abs32

CFLAGS = $(CPPFLAGS) $(BUILD_$(MACH)_FLAGS) $(ARCH_FLAGS) \
	-xO3 -xspace -W0,-Lt -v -c

LD_OPTIONS=

OBJECTS = hwc.o

# "env -" is used to strip LD_* variables from the environment, since our
# normal flags like -z text & -z defs are incompatible with building the
# kernel module with -r.
hwc: $(OBJECTS)
	env - /usr/ccs/bin/ld -r -o $@ $(OBJECTS)

hwc.o: hwc.c hwcio.h

install:
	mkdir -p $(DESTDIR)/kernel/strmod$(ARCHLIBSUBDIR)
	$(INSTALL) -m 755 hwc $(DESTDIR)/kernel/strmod$(ARCHLIBSUBDIR)
