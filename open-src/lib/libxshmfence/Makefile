###############################################################################
#
# libxshmfence
#
# Copyright (c) 2015, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libxshmfence

# Version number (used in path names)
MODULE_VERSION=1.2

# Checksums for upstream tarball
TARBALL_MD5   = 66662e76899112c0f99e22f2fc775a7e
TARBALL_SHA1  = a2ebe90e5595afca4db93a4359732af43b2b8c69
TARBALL_SHA256= d21b2d1fd78c1efbe1f2c16dae1cb23f8fd231dcf891465b8debe636a9054b0c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name (used for specfiles/mapfiles)
LIBNAME=xshmfence

# ARC cases that covered this module
# PSARC/2015/251 Xorg server 1.17
MODULE_ARC_CASES = PSARC/2015/251

MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP) 

# pkg-config files that need to have -R added
FIX_PC_FILES=xshmfence.pc.in

# Don't include path to x11proto headers in our proto area in the *.pc files.
MODULE_FIX_PC_FLAGS += -e 's| \@XPROTO_CFLAGS\@||g;'

include ../Makefile.inc
