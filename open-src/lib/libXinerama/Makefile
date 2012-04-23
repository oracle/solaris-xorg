###############################################################################
#
# libXinerama - Xlib-based client library for Xinerama extension protocol
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
MODULE_NAME=libXinerama

# Version number (used in path names)
MODULE_VERSION=1.1.2

# Checksums for upstream tarball
TARBALL_MD5   = cb45d6672c93a608f003b6404f1dd462
TARBALL_SHA1  = 06a5b636305725ce09f6c3a4d5a15f2f188b5afd
TARBALL_SHA256= a4e77c2fd88372e4ae365f3ca0434a23613da96c5b359b1a64bf43614ec06aac

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = sun-abi.patch

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
