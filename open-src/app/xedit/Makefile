###############################################################################
#
# xedit - simple Xaw-based text editor for X
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
MODULE_NAME=xedit

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=editor/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes
MODULE_DESC = the xedit text editor

# Package classification (override default)
MODULE_PKGCLASS=Development/Editors
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.2.2

# Checksums for upstream tarball
TARBALL_MD5  = 74ee6db901e81978b087b2026d8f30a0
TARBALL_SHA1 = f37b5d0f8ec461955f0419dbb1890d8cf63d179d
TARBALL_SHA256 = 69aa42885dfc06332ca22eb01cc7187e49206e6d65b74113a8ee4cc345fc2927

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xedit.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xedit

include ../Makefile.inc
