###############################################################################
#
# xscope - X11 protocol monitor/analyzer utility
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
MODULE_NAME=xscope

# Version number (used in path names)
MODULE_VERSION=1.3.1

# Checksums for upstream tarball
TARBALL_MD5   = 6b26a40429f868b726c4953eead03f0a
TARBALL_SHA1  = c0bf5bce04a3b46cb53d443ccedc5c6c393784aa
TARBALL_SHA256= 25e6db9a2d556c56797f9bb32cac650ac3afd130b49a1cea074694c2754c8582

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
