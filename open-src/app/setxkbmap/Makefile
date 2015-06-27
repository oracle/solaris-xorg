###############################################################################
#
# setxkbmap - utility for setting XKB layout & keymap options
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
MODULE_NAME=setxkbmap

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/keyboard/xkb-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the setxkbmap keyboard configuration utility

# Version number (used in path names)
MODULE_VERSION=1.3.1

# Checksums for upstream tarball
TARBALL_MD5   = 2c47a1b8e268df73963c4eb2316b1a89 
TARBALL_SHA1  = 560a02cf6f7db1f79739a4f2bd4f8050c7eb6f43
TARBALL_SHA256= a9ddb3963f263ba13f0ea105d8c45a531832140530217cc559587bb94f02d3e1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# ARC cases that covered this module
# PSARC/2008/162 setxkbmap
MODULE_ARC_CASES = PSARC/2008/162
# PSARC/2011/033 X11R7.6 application updates
MODULE_ARC_CASES += PSARC/2011/033
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES += PSARC/2015/221

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/setxkbmap.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = setxkbmap

include ../Makefile.inc


