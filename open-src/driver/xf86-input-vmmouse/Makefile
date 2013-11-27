###############################################################################
#
# xf86-input-vmmouse Makefile
#
# Copyright (c) 2007, 2013, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xf86-input-vmmouse

# Version number (used in path names)
MODULE_VERSION=13.0.0

# Checksums for upstream tarball
TARBALL_MD5   = 34f9f64ee6a1a51fc8266a9af24e1e07
TARBALL_SHA1  = 0fa67d2ca9b2b26d32b93cc030fc3ea28964fe81
TARBALL_SHA256= 04cfb60366008d4db815c550d8fb8d0a4270c75fa7a20fa3bddc9ecbd355612c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = libraries.patch,-p1

# Regenerate configure after patching src/Makefile.am in libraries.patch
AUTORECONF=yes

# vmmouse_client code won't build with Studio compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Need to use GNU Make to build due to Solaris make bug 15352407
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - need to set install locations for HAL config file & probe util/script
MODULE_CONFIG_OPTS= \
	--with-hal-bin-dir=/usr/sbin \
	--with-hal-callouts-dir=/usr/lib/hal \
	--with-hal-fdi-dir=/etc/hal/fdi/policy/10osvendor \
	--bindir=$(X11_SERVERLIBS_DIR)

# Extra flags to link driver, but not utility programs with
DRIVER_LD_FLAGS = $(DEFAULT_MODTYPE_LD_OPTIONS)
MODULE_CONFIG_OPTS += XORG_LIBS="$(DRIVER_LD_FLAGS)"
USE_DEFAULT_MODTYPE_LD_OPTIONS = no

# Override linker flags for programs
VMM_PROG_LDFLAGS=$(PROG_LDFLAGS) ../shared/libvmmouse.a
PROGRAM_VARS = vmmouse_detect
PROGRAM_MAKEFLAGS = $(PROGRAM_VARS:%=%_LDADD="$(VMM_PROG_LDFLAGS)")
MODULE_BUILD_MAKEFLAGS = AM_MAKEFLAGS='$(PROGRAM_MAKEFLAGS)'

include ../Makefile.inc
