###############################################################################
#
# xcursorgen - prepares X11 cursor sets for use with libXcursor
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
MODULE_NAME=xcursorgen

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=image/xcursorgen
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Applications/Graphics and Imaging
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5   = 09f56978a62854534deacc8aa8ff3031
TARBALL_SHA1  = 1d0ca82422d328522b06b218f86b5901a7d5521b
TARBALL_SHA256= bc7171a0fa212da866fca2301241630e2009aea8c4ddb75f21b51a31c2e4c581

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xcursorgen.man
MODULE_STABILITY=Volatile

# ARC cases that covered this module
# LSARC/2007/491 libXcursor
MODULE_ARC_CASES = LSARC/2007/491

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS = LIB_MAN_SUFFIX=3Xcursor

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xcursorgen

include ../Makefile.inc
