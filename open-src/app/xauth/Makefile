###############################################################################
#
# xauth - utility for managing cookies in .Xauthority files
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
MODULE_NAME=xauth

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/session/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xauth utility for managing cookies in .Xauthority files

# Version number (used in path names)
MODULE_VERSION=1.0.10

# Checksums for upstream tarball
TARBALL_MD5   = f3f76cb10f69b571c43893ea6a634aa4
TARBALL_SHA1  = 87946b2af3ff13705d8eb60adae5c0bcdf839967
TARBALL_SHA256= 5afe42ce3cdf4f60520d1658d2b17face45c74050f39af45dccdc95e73fafc4d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xauth.man
MODULE_STABILITY=Committed

# ARC cases that covered this module
# PSARC/2004/469 xauth -n
MODULE_ARC_CASES = PSARC/2004/469

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xauth

include ../Makefile.inc
