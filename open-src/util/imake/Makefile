###############################################################################
#
# imake - Legacy X11 build configuration and makefile generation tool
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
MODULE_NAME=imake

# Package metadata
MODULE_DESC = the imake makefile generation tool

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5   = 4042a4139a4636f78e2b8a144fdd1fcd
TARBALL_SHA1  = 52e236776133f217d438622034b8603d201a6ec5
TARBALL_SHA256= 690c2c4ac1fad2470a5ea73156cf930b8040dc821a0da4e322014a42c045f37e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Uncommitted

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS  = --enable-mkdirhier \
 --disable-ccmakedep --disable-cleanlinks --disable-makeg \
 --disable-mergelib --disable-mkhtmlindex --disable-revpath

# Prefer cpp from /usr/lib
MODULE_CONFIG_OPTS += RAWCPP=/usr/lib/cpp

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = imake xmkmf mkdirhier

### Include common rulesets
include ../Makefile.inc

