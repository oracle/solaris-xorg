###############################################################################
#
# mkfontdir - legacy X11 core font metadata creation tool
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
MODULE_NAME=mkfontdir

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/font-utilities
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5   = 18c429148c96c2079edda922a2b67632
TARBALL_SHA1  = 3c06dad8a5fbf7362b51fb7d6b1ab805eba40336
TARBALL_SHA256= 56d52a482df130484e51fd066d1b6eda7c2c02ddbc91fe6e2be1b9c4e7306530

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# ARC cases that covered this module
# LSARC/2008/009 X11R7 font server and utilities
MODULE_ARC_CASES = LSARC/2008/009

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/mkfontdir.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = mkfontdir

include ../Makefile.inc
