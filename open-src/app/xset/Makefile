###############################################################################
#
# xset 1.x Makefile
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xset

# Version number (used in path names)
MODULE_VERSION=1.2.2

# Checksums for upstream tarball
TARBALL_MD5  = d44e0057d6722b25d5a314e82e0b7e7c
TARBALL_SHA1 = e48d628259dbe1ff413ec8408b4fee9561c8815c
TARBALL_SHA256 = 61371c140030b8b05075a1378b34a4d7c438ed9159496a95f10782c6f4aec1e8

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = xsetfbpm.patch,-p1

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# PSARC 1999/188 - FBPM (Frame Buffer Power Management) Extension
MODULE_ARC_CASES = PSARC/1999/188
# LSARC/2009/361 xset support for XKB indicator names
MODULE_ARC_CASES += LSARC/2009/361


# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xset.man
MODULE_STABILITY=Uncommitted

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xset

include ../Makefile.inc
