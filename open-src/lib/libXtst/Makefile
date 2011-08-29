###############################################################################
#
# Xtst library Makefile
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXtst

# Version number (used in path names)
MODULE_VERSION=1.2.0

# Checksums for upstream tarball
TARBALL_MD5  = 7c592c72da6676f8b0aeec9133b81686
TARBALL_SHA1 = 9fb06ed599caf7f9e7115cbbfadf02b47c17aa72

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	solaris-abi.patch

# Need to regenerate automake files after solaris-abi.patch
AUTORECONF=yes

# Library name
LIBNAME=Xtst

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3xtst

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# configure includes xi in PKG_CHECK_MODULES since libXi headers & macros
# are used, but currently no functions are actually linked with, so we can
# ignore it when linking
MODULE_LD_OPTIONS += -z ignore

# pkg-config files that need to have -R added
FIX_PC_FILES=xtst.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXtst.so libXtst.so.1

include ../Makefile.inc

