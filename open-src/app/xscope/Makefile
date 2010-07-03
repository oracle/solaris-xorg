###############################################################################
#
# xscope 1.x Makefile
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xscope

# Version number (used in path names)
MODULE_VERSION=1.2

# Checksums for upstream tarball
TARBALL_MD5  = 1a9df0ac0dd5a2c2a7948dcf48d126c3
TARBALL_SHA1 = 3952f381c3f8ee81cd602e6f025304c2322a06a0

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# LSARC/2009/549 xscope 1.2
MODULE_ARC_CASES = LSARC/2009/549

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xscope

include ../Makefile.inc
