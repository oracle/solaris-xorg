###############################################################################
#
# xtrans 1.x Makefile
#
# Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Package classification (override default)
MODULE_PKGCLASS=Development/X11
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.2.6

# Checksums for upstream tarball
TARBALL_MD5  = c66f9ffd2da4fb012220c6c40ebc7609
TARBALL_SHA1 = 99f019a6e6d4d2b8274c4a27f9c8482c7fc64e85

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = inetv6.patch \
	tsol-unix-domain.patch

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
