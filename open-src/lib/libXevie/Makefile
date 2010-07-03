###############################################################################
#
# Xevie 1.x Makefile
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXevie

# Version number (used in path names)
MODULE_VERSION=1.0.2

# Checksums for upstream tarball
TARBALL_MD5  = 88e9bf535364e7c821ef4014fb1ca2dd
TARBALL_SHA1 = d22fd24d8f096fdff54d303d63fea1422f0cfeea

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=Xevie

# ARC cases that covered this module
# LSARC/2002/312 XEvIE
MODULE_ARC_CASES = LSARC/2002/312
# PSARC/2008/194 X11R7 update to libXext, libXevie & libXss
MODULE_ARC_CASES += PSARC/2008/194

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xevie

# libXevie actually doesn't call any libc functions
NEED_LIBC=no

# pkg-config files that need to have -R added
FIX_PC_FILES=xevie.pc.in

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXevie.so libXevie.so.1

include ../Makefile.inc
