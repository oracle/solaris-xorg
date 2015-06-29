###############################################################################
#
# libxtrans - shared source code for TCP/pipe/socket transport
#
# Copyright (c) 2006, 2015, Oracle and/or its affiliates. All rights reserved.
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

# IMPORTANT NOTE:  xtrans is not really a library, but shared source code
# compiled into other modules at build time.

# Package name used in tarballs
MODULE_NAME=xtrans

# Package metadata
GIT_URL     = $(XORG_GIT_URL)/lib/libxtrans
GIT_URL_SET = yes

# Package classification (override default)
MODULE_PKGCLASS=Development/X11
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.3.5

# Checksums for upstream tarball
TARBALL_MD5   = c5ba432dd1514d858053ffe9f4737dd8
TARBALL_SHA1  = 2d3ae1839d841f568bc481c6116af7d2a9f9ba59
TARBALL_SHA256= adbd3b36932ce4c062cd10f57d78a156ba98d618bdb6f50664da327502bc8301

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = inetv6.patch,-p1 \
	7162172.patch
SOURCE_PATCHES += tsol-unix-domain.patch,-p1

# Library name (used for specfiles/mapfiles)
LIBNAME=xtrans

# Man pages to apply Sun footer to & attributes to list
# - Xtrans has none

# Because xtrans is not a real library, don't need 64-bit build,
# and the pkg-config file goes into share directory
BUILD_TYPES=32
BUILD_TYPES_SET=yes

MODULE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODULE_PKGCONFIG_DIR_SET=yes

include ../Makefile.inc
