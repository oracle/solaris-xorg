# -*- Makefile -*- rules common to all X.Org drivers
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
# @(#)Makefile.inc	1.13	08/11/26
#

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=driver

### Include common definitions
include $(TOP)/common/Makefile.init

# Build 64-bit on SPARC, 32-bit & 64-bit on x86, to match Xorg server builds
BUILD_TYPES_SET=yes
BUILD_TYPES_sparc=64
BUILD_TYPES_i386=32 64
BUILD_TYPES=$(BUILD_TYPES_$(MACH))

# Additional command line options to GNU autoconf configure script
MODTYPE_CONFIG_OPTS = \
	--with-xorg-module-dir='$${libdir}/modules$(SERVERMOD_subdir)'

MODTYPE_BUILD_32_FLAGS=ARCHBINSUBDIR="$(SUBDIR32)"
MODTYPE_BUILD_64_FLAGS=ARCHBINSUBDIR="$(SUBDIR64)"

# Set comments in ELF binaries to help identify versions/builds
MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

MODTYPE_CPPFLAGS= \
	-I$(PROTODIR)/usr/X11/include/xorg	\
	-I$(PROTODIR)/usr/X11/include/drm	\
	-I$(PROTODIR)/usr/X11/include/X11/dri

# Inline assembly in i386 requires gcc until we move to Studio 12 or later
MODTYPE_COMPILER_sparc=suncc
MODTYPE_COMPILER_i386=gcc
MODTYPE_COMPILER=$(MODTYPE_COMPILER_$(MACH))
MODTYPE_COMPILER_SET=yes

MODTYPE_CFLAGS=$(PROG_CFLAGS)

MODTYPE_LDFLAGS_cc=
MODTYPE_LDFLAGS_gcc=-R/usr/sfw/lib$(ARCHLIBSUBDIR)
MODTYPE_LDFLAGS=$(LDFLAGS) -Wl,-B,direct $(MODTYPE_LDFLAGS_$(MODULE_COMPILER))

MODTYPE_LD_OPTIONS=-z defs $(X11_SERVERLIBS_LDFLAGS) \
	-M $(TOP)/xserver/xorg/$(BUILD_DIR)/mapfile-Xorg-externs -lc

# Need to fix libtool interference with our linker flags
DELIBTOOLIZE=yes

### Include common rules
include $(TOP)/common/Makefile.inc
