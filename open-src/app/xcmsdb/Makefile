###############################################################################
#
# xcmsdb - Device Color Characterization utility
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
MODULE_NAME=xcmsdb

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xcmsdb Device Color Characterization utility

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5   = 9956d751ea3ae4538c3ebd07f70736a0
TARBALL_SHA1  = b1d09b1a9a4324fa86c85340ae47cc34743423a9
TARBALL_SHA256= e5585361bb8b6a05bb814a8d0e444ee93e0f00180881d3070aff4571e97f67c6

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xcmsdb.man
MODULE_STABILITY=Committed

# ARC cases that covered this module
# PSARC/1992/173 - Update MIT-based Clients to X11R5 Level
MODULE_ARC_CASES = PSARC/1992/173
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES += PSARC/2015/221

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xcmsdb

include ../Makefile.inc
