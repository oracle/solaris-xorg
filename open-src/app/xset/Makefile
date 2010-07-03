###############################################################################
#
# xset 1.x Makefile
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
MODULE_NAME=xset

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5  = eeb0d02f69c76bd40470dede99b4bd49
TARBALL_SHA1 = ff4295442821826092b17248c1ad65e16cd860ef

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = xsetfbpm.patch xsetman.patch

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/utility/x11-server-utilities
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# PSARC 1999/188 - FBPM (Frame Buffer Power Management) Extension
MODULE_ARC_CASES = PSARC/1999/188
# LSARC/2009/361 xset support for XKB indicator names
MODULE_ARC_CASES += LSARC/2009/361


# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Uncommitted

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xset

include ../Makefile.inc
