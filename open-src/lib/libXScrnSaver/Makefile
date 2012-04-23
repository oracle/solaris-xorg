###############################################################################
#
# libXScrnSaver - Xlib-based client library for MIT-SCREEN-SAVER extension
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXScrnSaver

# Version number (used in path names)
MODULE_VERSION=1.2.2

# Checksums for upstream tarball
TARBALL_MD5   = 7a773b16165e39e938650bcc9027c1d5
TARBALL_SHA1  = 7b8298eec371c33a71232e3653370a98f03c6c88
TARBALL_SHA256= 8ff1efa7341c7f34bcf9b17c89648d6325ddaae22e3904e091794e0b4426ce1d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=Xss

# ARC cases that covered this module
# PSARC/2008/194 X11R7 update to libXext, libXevie & libXss
MODULE_ARC_CASES = PSARC/2008/194

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3xss

# pkg-config files that need to have -R added
FIX_PC_FILES=xscrnsaver.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXss.so libXss.so.1

include ../Makefile.inc
