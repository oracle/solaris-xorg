###############################################################################
#
# libXres - Xlib-based client library for Xinerama extension protocol
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
MODULE_NAME=libXres

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5   = 45ef29206a6b58254c81bea28ec6c95f
TARBALL_SHA1  = 45e09c6b771af5d8c448ce31de65d15bf83b7579
TARBALL_SHA256= 26899054aa87f81b17becc68e8645b240f140464cf90c42616ebb263ec5fa0e5

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name (used for specfiles/mapfiles)
LIBNAME=XRes

# ARC cases that covered this module
# LSARC/2003/686 X-Resource Extension
MODULE_ARC_CASES = LSARC/2003/686

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xres

# Package metadata
GIT_URL     = $(XORG_GIT_URL)/lib/libXRes
GIT_URL_SET = yes

# pkg-config files that need to have -R added
FIX_PC_FILES=xres.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXRes.so libXRes.so.1

include ../Makefile.inc
