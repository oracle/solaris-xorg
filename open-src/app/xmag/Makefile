###############################################################################
#
# Copyright (c) 2008, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xmag

# Package metadata
MODULE_DESC = the xmag screen magnification utility

# Version number (used in path names)
MODULE_VERSION=1.0.6

# Checksums for upstream tarball
TARBALL_MD5  = 8aaa41374935d697ee55d7dc9de70781
TARBALL_SHA1 = 77c4b23765a1a30afde46a6e57afd77f20d58185
TARBALL_SHA256 = 87a2bc23b251e2d8f8370d3283a4d6c8dac98a30cb5749a04336cdb55c14e161 

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Allow linker to skip linking to libm if compiler inlined all math functions
MODULE_LDFLAGS = $(ZDISCARD_UNUSED_DEP)

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xmag

include ../Makefile.inc
