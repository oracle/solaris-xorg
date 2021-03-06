###############################################################################
#
# libXres - Xlib-based client library for Xinerama extension protocol
#
# Copyright (c) 2008, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.2.0

# Checksums for upstream tarball
TARBALL_MD5   = 5d6d443d1abc8e1f6fc1c57fb27729bb
TARBALL_SHA1  = b6996ab05460a3b7311f02da23ddf117a5442fd9
TARBALL_SHA256= ff75c1643488e64a7cfbced27486f0f944801319c84c18d3bd3da6bf28c812d4

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name (used for specfiles/mapfiles)
LIBNAME=XRes

# ARC cases that covered this module
# LSARC/2003/686 X-Resource Extension
MODULE_ARC_CASES = LSARC/2003/686
# libXres 1.2.0
MODULE_ARC_CASES += PSARC/2017/229

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
