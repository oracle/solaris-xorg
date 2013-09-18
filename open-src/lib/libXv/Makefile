###############################################################################
#
# libXv - Xlib-based client library for XVideo extension protocol
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXv

# Version number (used in path names)
MODULE_VERSION=1.0.10

# Checksums for upstream tarball
TARBALL_MD5   = e0af49d7d758b990e6fef629722d4aca
TARBALL_SHA1  = 1e93df036bb2bb01e85b7c8886760affb33b8e88
TARBALL_SHA256= 55fe92f8686ce8612e2c1bfaf58c057715534419da700bda8d517b1d97914525

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name
LIBNAME=Xv

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xv

# pkg-config files that need to have -R added
FIX_PC_FILES=xv.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXv.so libXv.so.1

include ../Makefile.inc
