###############################################################################
#
# libXau - library for manipulating .Xauthority files
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXau

# Version number (used in path names)
MODULE_VERSION=1.0.8

# Checksums for upstream tarball
TARBALL_MD5   = 685f8abbffa6d145c0f930f00703b21b
TARBALL_SHA1  = d9512d6869e022d4e9c9d33f6d6199eda4ad096b
TARBALL_SHA256= fdd477320aeb5cdd67272838722d6b7d544887dfe7de46e1e7cc0c27c2bea4f2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=Xau

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/Xau.man
MODULE_STABILITY=Committed
MODULE_MTLEVEL=Unsafe
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3xau

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --enable-xthreads --with-lint --enable-lint-library

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xau.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXau.so libXau.so.6

include ../Makefile.inc
