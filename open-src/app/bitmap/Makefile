###############################################################################
#
# bitmap (XBM format) editor application
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
MODULE_NAME=bitmap

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=image/editor/bitmap
MODULE_PKGNAME_SET=yes
MODULE_DESC = the bitmap, bmtoa, and atobm utilities

# Package classification (override default)
MODULE_PKGCLASS=Applications/Graphics and Imaging
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5   = 9c18cc1048146e29d68bfa9d0348b11d
TARBALL_SHA1  = 18c4762b8c94b519ee2ca6c3171cc2f85e6b2421
TARBALL_SHA256= 7ea2823a930ddfe3bfc0c7fbb16a5b463222edf05284c5cdc40a6d3c5c3988af

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/bitmap.man man/atobm.man man/bmtoa.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = bitmap atobm bmtoa

# ignore some unneeded dependencies that upstream includes:
# unreferenced object=/usr/lib/libXt.so.4; unused dependency of /usr/bin/bmtoa
# unreferenced object=/usr/lib/libX11.so.4; unused dependency of /usr/bin/bmtoa
MODULE_LDFLAGS = -z ignore

include ../Makefile.inc
