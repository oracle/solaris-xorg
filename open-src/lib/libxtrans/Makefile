###############################################################################
#
# xtrans 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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

# Version number (used in path names)
MODULE_VERSION=1.2.5

# Checksums for upstream tarball
TARBALL_MD5  = 2d1e57e82acc5f21797e92341415af2f
TARBALL_SHA1 = e8c4ce58b42d05e613fe535319a7d7f45f30f3e9

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = inetv6.patch \
	tsol-unix-domain.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=xtrans

# Man pages to apply Sun footer to & attributes to list
# - Xtrans has none

# Xtrans doesn't build a library, so use the pkgconfig file it builds instead
LIB_BUILT = xtrans.pc

include ../Makefile.inc

