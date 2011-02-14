###############################################################################
#
# Xft 2.x Makefile
#
# Copyright (c) 2005, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXft

# Version number (used in path names)
MODULE_VERSION=2.2.0

# Checksums for upstream tarball
TARBALL_MD5  = cce3c327258116493b753f157e0360c7
TARBALL_SHA1 = ed29784259f4e26df78141035560ae8a7c62e83f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xftcore-6261221.patch \
	6872780.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xft

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/Xft.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3

# ARC cases that covered this module
# LSARC/2003/274 Xft2 library
MODULE_ARC_CASES = LSARC/2003/274

# Mapfile used to control which symbols are exported
XFT_MAPFILE=$(PWD)/mapfile.scope
MODULE_LD_OPTIONS = -M$(XFT_MAPFILE)

# pkg-config files that need to have -R added
FIX_PC_FILES=xft.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXft.so libXft.so.2

include ../Makefile.inc

