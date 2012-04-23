###############################################################################
#
# mkfontscale - legacy X11 core font metadata creation tool
#
# Copyright (c) 2006, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=mkfontscale

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/font-utilities
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5   = 414fcb053418fb1418e3a39f4a37e0f7
TARBALL_SHA1  = ba021369ae3b364ba083b9eb5e570e54b21fd583
TARBALL_SHA256= ce55f862679b8ec127d7f7315ac04a8d64a0d90a0309a70dc56c1ba3f9806994

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# ARC cases that covered this module
# LSARC/2008/009 X11R7 font server and utilities
MODULE_ARC_CASES = LSARC/2008/009

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/mkfontscale.man
MODULE_STABILITY=Uncommitted

# Additional configure script options
MODULE_CONFIG_OPTS= --with-bzip2

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = mkfontscale

# Need to find libfontenc & libfreetype in matching directory when used
# in alternate root environments, like during the build of the fonts
MODULE_LD_OPTIONS= -R $$ORIGIN/$(X11_SERVERLIBS_DIR:$(X11_DIR)%=..%)$(ARCHLIBSUBDIR)
include ../Makefile.inc
