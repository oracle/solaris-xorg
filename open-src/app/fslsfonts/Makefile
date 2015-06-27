###############################################################################
#
# fslsfonts utility to list fonts available from an xfs font server
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
MODULE_NAME=fslsfonts

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/xfs/xfs-utilities
MODULE_PKGNAME_SET=yes
MODULE_DESC = the fslsfonts utility to list fonts from an xfs font server

# ARC cases that covered this module
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES = PSARC/2015/221

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5   = 6eb6bbd0a9a733e8ad0d497383bc0232
TARBALL_SHA1  = 0a0d37c65a8cdd3aff22d633b5ed164e864929d4
TARBALL_SHA256= f1ed55093356b78daec2678d22f52d42ec11fe88575562d734396962e626d7f6

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/fslsfonts.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = fslsfonts

include ../Makefile.inc
