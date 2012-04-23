###############################################################################
#
# xpr - utility to print xwd window dump images
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xpr

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-window-dump
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.4

# Checksums for upstream tarball
TARBALL_MD5   = 6101f04731ffd40803df80eca274ec4b
TARBALL_SHA1  = 0645404e9aa2c9f979e4d006c483633bf6cfc2d9
TARBALL_SHA256= fed98df31eb93d3dca4688cb535aabad06be572e70ace3b1685679c18dd86cb5

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += 6724753.patch,-p1

# Allow linker to skip linking to libm if compiler inlined all math functions
MODULE_LDFLAGS = -z ignore

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Committed

# ARC cases that covered this module
# PSARC/2002/435 xpr Paper Size Option
MODULE_ARC_CASES = PSARC/2002/435

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xpr xdpr

include ../Makefile.inc
