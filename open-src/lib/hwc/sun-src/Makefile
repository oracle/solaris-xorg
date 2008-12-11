###############################################################################
#
# hwc 1.x Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#
# @(#)Makefile	1.2	08/12/10
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
