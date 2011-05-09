###############################################################################
#
# xcb-util-wm Makefile
#
# Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xcb-util-wm

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/xcb-util
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=0.3.8

# Checksums for upstream tarball
TARBALL_MD5  = dda47289bc846a6a3e07824e9ec3aef8
TARBALL_SHA1 = e7684921870a62c9e4f53ff8376a468206dc977f

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=xcbutil

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted

# Need to use GNU Make to build, due to the use of $< in explicit rules
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

include ../Makefile.inc
