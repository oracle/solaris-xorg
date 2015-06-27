###############################################################################
#
# sessreg - tool for X display manager session registration in utmpx
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
MODULE_NAME=sessreg

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/session/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5   = e238c89dabc566e1835e1ecb61b605b9
TARBALL_SHA1  = a27a476f7f39ae30a16dfa25ca07c12378cff7f0
TARBALL_SHA256= 551177657835e0902b5eee7b19713035beaa1581bbd3c6506baa553e751e017c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/sessreg.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = sessreg

include ../Makefile.inc
