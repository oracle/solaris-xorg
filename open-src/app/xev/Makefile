###############################################################################
#
# xev - X11 event dumping tool
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
MODULE_NAME=xev

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.2.0

# Checksums for upstream tarball
TARBALL_MD5   = 2727c72f3eba0c23f8f6b2e618d195a2
TARBALL_SHA1  = 6b05706ed39382b5d08c5351c00ba7e127709eec
TARBALL_SHA256= 3786a77dab17741d508d5d117a0ff33bb6eabf93e0935388b5f920bfcf2fb38f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# ARC cases that covered this module
# Original integration predates ARC
# PSARC/2011/033 X11R7.6 application updates
MODULE_ARC_CASES = PSARC/2011/033

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xev.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xev

include ../Makefile.inc
