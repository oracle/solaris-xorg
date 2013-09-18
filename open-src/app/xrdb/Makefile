###############################################################################
#
# xrdb - X server resource database utility
#
# Copyright (c) 2008, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xrdb

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xrdb X server resource database utility

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5   = b54c7e3e53b4f332d41ed435433fbda0
TARBALL_SHA1  = b95ae53b767ee7b56baa55fc78eb9c0b9e5ccd29
TARBALL_SHA256= 73827b6bbfc9d27ca287d95a1224c306d7053cd7b8156641698d7dc541ca565b

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xrdb.man
MODULE_STABILITY=Committed

# Specify which cpp to use instead of depending on builder's $PATH
MODULE_CONFIG_OPTS = --with-cpp=/usr/lib/cpp

MODULE_CPPFLAGS=-DPATHETICCPP

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xrdb

include ../Makefile.inc
