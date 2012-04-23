###############################################################################
#
# libFS - client library to connect to xfs font servers
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
MODULE_NAME=libFS

# Version number (used in path names)
MODULE_VERSION=1.0.4

# Checksums for upstream tarball
TARBALL_MD5   = 645f83160cf7b562734e2038045106d1
TARBALL_SHA1  = f114f32f60eb84d3e1d0e6f0dfe7679b07e91295
TARBALL_SHA256= 7073761e7594d43180a922605fb64cce60e5ccb8c06f8efa24f2d4621f5e8315

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += solaris-abi.patch,-p1
SOURCE_PATCHES += lint.patch,-p1

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Library name
LIBNAME=FS

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3FS

# pkg-config files that need to have -R added
FIX_PC_FILES=libfs.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libFS.so libFS.so.5

include ../Makefile.inc
