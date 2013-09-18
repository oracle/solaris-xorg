###############################################################################
#
# libXinerama - Xlib-based client library for Xinerama extension protocol
#
# Copyright (c) 2007, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXinerama

# Version number (used in path names)
MODULE_VERSION=1.1.3

# Checksums for upstream tarball
TARBALL_MD5   = 9336dc46ae3bf5f81c247f7131461efd
TARBALL_SHA1  = 6a3956d73f7457ef0b4db12806d99c6918e328e3
TARBALL_SHA256= 7a45699f1773095a3f821e491cbd5e10c887c5a5fce5d8d3fced15c2ff7698e2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = sun-abi.patch,-p1

# Library name
LIBNAME=Xinerama

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3xinerama

# ARC cases that covered this module
# PSARC/2007/135 libXinerama
MODULE_ARC_CASES = PSARC/2007/135

# Needed for sun-abi.patch
MODULE_CPPFLAGS= -D SUNSOFT

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xinerama.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXinerama.so libXinerama.so.1

include ../Makefile.inc
