###############################################################################
#
# xlsatoms - utility to list atoms defined on a given X server
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
MODULE_NAME=xlsatoms

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/x11-info-clients
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xlsatoms utility to list atoms defined on a given X server

# ARC cases that covered this module
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES = PSARC/2015/221

# Version number (used in path names)
MODULE_VERSION=1.1.2

# Checksums for upstream tarball
TARBALL_MD5   = 5dcb6e6c4b28c8d7aeb45257f5a72a7d
TARBALL_SHA1  = 0946410a2cd02b0dee7809fd75be090fb3961e9c
TARBALL_SHA256= 47e5dc7c3dbda6db2cf8c00cedac1722835c1550aa21cfdbc9ba83906694dea4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xlsatoms.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xlsatoms

include ../Makefile.inc
