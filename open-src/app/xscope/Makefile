###############################################################################
#
# xscope - X11 protocol monitor/analyzer utility
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
MODULE_NAME=xscope

# Version number (used in path names)
MODULE_VERSION=1.4.1

# Checksums for upstream tarball
TARBALL_MD5   = ad80eb4ec8d807eb542c4e790c90e848
TARBALL_SHA1  = d1da41796e72ec41b7763ce0370182e7a33cdbfd
TARBALL_SHA256= 33807fbd51822435b23c28e7e78116e552826a6ca4d85632f098ad0ddf1ff423

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# LSARC/2009/549 xscope 1.2
MODULE_ARC_CASES = LSARC/2009/549

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xscope.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xscope

# Since xscope allocates a 32k buffer for FD_SETSIZE file descriptors,
# we want far fewer than the 1024 default in 32-bit builds or the 
# 65536 default in 64-bit builds - working to make this dynamic upstream.
FD_SETSIZE=256
FD_SETSIZE_SET=yes

include ../Makefile.inc
