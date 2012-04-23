###############################################################################
#
# xinit - starts a X server & session without a display manager
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
MODULE_NAME=xinit

# Version number (used in path names)
MODULE_VERSION=1.3.2

# Checksums for upstream tarball
TARBALL_MD5   = 9c0943cbd83e489ad1b05221b97efd44
TARBALL_SHA1  = 1c287ac016d277f973652fa3707eef6db8353d5a
TARBALL_SHA256= a1867fdaa83f68750b12ba4305c3c62f5992d0f52cfeb98e96c27a8e690e0235

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 6714067.patch,-p1 7023672.patch,-p1

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/session/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# PSARC/2008/379  startx
MODULE_ARC_CASES = PSARC/2008/379
# PSARC/2011/033 X11R7.6 application updates
MODULE_ARC_CASES += PSARC/2011/033

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xinit startx

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS = --with-xinitdir=/etc/X11/xinit

include ../Makefile.inc
