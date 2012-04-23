###############################################################################
#
# libXtst - Xlib-based client library for Xtest & Record extension protocols
#
# Copyright (c) 2007, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.2.1

# Checksums for upstream tarball
TARBALL_MD5   = e8abc5c00c666f551cf26aa53819d592
TARBALL_SHA1  = 44e19899f9938d3db5546e51c338d6f86d88cfe2
TARBALL_SHA256= 7eea3e66e392aca3f9dad6238198753c28e1c32fa4903cbb7739607a2504e5e0

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

